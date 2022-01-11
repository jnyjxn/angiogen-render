#include <cfloat>

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               SceneGraphNode* apParent):
        m_label(aLabel),
		m_p_geometry(0),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::SceneGraphNode(const SceneGraphNode& aNode):
		m_label(aNode.m_label),
		m_local_geometry(aNode.m_local_geometry),
		m_p_geometry(0),
		m_local_transformation(aNode.m_local_transformation),
		m_global_transformation(aNode.m_global_transformation),
		m_position(aNode.m_position),
		m_p_parent(aNode.m_p_parent),
		m_p_children(aNode.m_p_children),
		m_world_properties_up_to_date(aNode.m_world_properties_up_to_date)
{
	if (aNode.m_p_geometry != &aNode.m_local_geometry)
	{
		m_p_geometry = aNode.m_p_geometry;
	}
	else
	{
		m_p_geometry = &m_local_geometry;
	}
}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const PolygonMesh& aPolygonMesh,
                               const VEC3& aPosition,
                               SceneGraphNode* apParent):
        m_label(aLabel),
        m_local_geometry(aPolygonMesh),
		m_p_geometry(&m_local_geometry),
        m_position(aPosition),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               PolygonMesh* apPolygonMesh,
                               const VEC3& aPosition,
                               SceneGraphNode* apParent):
        m_label(aLabel),
		m_p_geometry(apPolygonMesh),
        m_position(aPosition),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::~SceneGraphNode()
{
	//std::cout << "Delete node: " << m_label << std::endl;

    for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if (*ite)
    	{
    		delete *ite;
    	}
    }

    m_p_children.clear();
}


SceneGraphNode& SceneGraphNode::operator=(const SceneGraphNode& aNode)
{
	m_label = aNode.m_label;
	m_local_geometry = aNode.m_local_geometry;

	if (aNode.m_p_geometry != &aNode.m_local_geometry)
	{
		m_p_geometry = aNode.m_p_geometry;
	}
	else
	{
		m_p_geometry = &m_local_geometry;
	}

	m_local_transformation = aNode.m_local_transformation;
	m_global_transformation = aNode.m_global_transformation;
	m_position = aNode.m_position;
	m_p_parent = aNode.m_p_parent;
	m_p_children = aNode.m_p_children;
	m_world_properties_up_to_date = aNode.m_world_properties_up_to_date;

	return (*this);
}


void SceneGraphNode::display()
{
	if (m_p_geometry) m_p_geometry->display();
}


void SceneGraphNode::displayWireFrame()
{
	if (m_p_geometry) m_p_geometry->displayWireFrame();
}

const SceneGraphNode* SceneGraphNode::getParent() const
{
    return (m_p_parent);
}


unsigned int SceneGraphNode::getNumberOfChildren() const
{
    return (m_p_children.size());
}


SceneGraphNode& SceneGraphNode::getChild(unsigned int i)
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (*m_p_children[i]);
}


const SceneGraphNode& SceneGraphNode::getChild(unsigned int i) const
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (*m_p_children[i]);
}



const std::string& SceneGraphNode::getLabel() const
{
    return (m_label);
}


void SceneGraphNode::setGeometry(const PolygonMesh& aPolygonMesh)
{
	m_local_geometry = aPolygonMesh;
	m_p_geometry = &m_local_geometry;
}


void SceneGraphNode::setGeometry(PolygonMesh* apPolygonMesh)
{
	m_p_geometry = apPolygonMesh;
}


PolygonMesh& SceneGraphNode::getPolygonMesh()
{
	if (m_p_geometry)
		{
		return (*m_p_geometry);
		}
	else
		{
		return (m_local_geometry);
		}
}


const PolygonMesh& SceneGraphNode::getPolygonMesh() const
{
	if (m_p_geometry)
		{
		return (*m_p_geometry);
		}
	else
		{
		return (m_local_geometry);
		}
}


MATRIX4 SceneGraphNode::getLocalTransformationMatrix() const
{
    return (m_local_transformation * MATRIX4::buildTranslationMatrix(m_position));
}


MATRIX4 SceneGraphNode::getWorldTransformationMatrix()
{
	if (!m_world_properties_up_to_date)
	{
		updateWorldMatrix();
	}

    return (m_global_transformation);
}


std::map<RATIONAL_NUMBER, VEC3> SceneGraphNode::rayIntersect(
		const VEC3& aRayOrigin,
		const VEC3& aRayDirection,
		const MATRIX4& aTransformationMatrix)
{
	return getPolygonMesh().rayIntersect(
			aRayOrigin,
			aRayDirection,
			getWorldTransformationMatrix() * aTransformationMatrix);
}


void SceneGraphNode::setLocalTransformation(const MATRIX4& aMatrix)
{
	m_local_transformation = aMatrix;


	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_node->m_world_properties_up_to_date = false;
		p_waiting_list.pop_back();

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}
}


void SceneGraphNode::applyLocalTransformation(const MATRIX4& aMatrix)
{
    m_local_transformation = m_local_transformation * aMatrix;

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_node->m_world_properties_up_to_date = false;
		p_waiting_list.pop_back();

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}
}


void SceneGraphNode::addChild(const std::string& aLabel)
{
	m_p_children.push_back(new SceneGraphNode(aLabel, this));
}


void SceneGraphNode::addChild(const std::string& aLabel,
                              const PolygonMesh& aPolygonMesh,
                              const VEC3& aPosition)
{
    m_p_children.push_back(new SceneGraphNode(aLabel, aPolygonMesh, aPosition, this));
}


void SceneGraphNode::addChild(const std::string& aLabel,
		PolygonMesh* apPolygonMesh,
		const VEC3& aPosition)
{
    m_p_children.push_back(new SceneGraphNode(aLabel, apPolygonMesh, aPosition, this));
}


VEC3 SceneGraphNode::getNodeAndChildrenWorldMinCorner()
{
	VEC3 min_bbox(FLT_MAX, FLT_MAX, FLT_MAX);

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_waiting_list.pop_back();


		if (p_node->getPolygonMesh().getVertexNumber())
		{
			// Process the node
			VEC3 node_min_bbox = p_node->getWorldTransformationMatrix() * p_node->getPolygonMesh().getLocalMinCorner();

			if (min_bbox[0] > node_min_bbox[0]) min_bbox[0] = node_min_bbox[0];
			if (min_bbox[1] > node_min_bbox[1]) min_bbox[1] = node_min_bbox[1];
			if (min_bbox[2] > node_min_bbox[2]) min_bbox[2] = node_min_bbox[2];
			}

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}

	return (min_bbox);
}


VEC3 SceneGraphNode::getNodeAndChildrenWorldMaxCorner()
{
	VEC3 max_bbox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_waiting_list.pop_back();

		if (p_node->getPolygonMesh().getVertexNumber())
		{
			// Process the node
			VEC3 node_max_bbox = p_node->getWorldTransformationMatrix() * p_node->getPolygonMesh().getLocalMaxCorner();

			if (max_bbox[0] < node_max_bbox[0]) max_bbox[0] = node_max_bbox[0];
			if (max_bbox[1] < node_max_bbox[1]) max_bbox[1] = node_max_bbox[1];
			if (max_bbox[2] < node_max_bbox[2]) max_bbox[2] = node_max_bbox[2];
		}

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}

	return (max_bbox);
}


VEC3 SceneGraphNode::getNodeOnlyWorldMinCorner()
{
	VEC3 min_bbox(FLT_MAX, FLT_MAX, FLT_MAX);

	if (getPolygonMesh().getVertexNumber())
	{
		// Process the node
		VEC3 node_min_bbox = getWorldTransformationMatrix() * getPolygonMesh().getLocalMinCorner();

		if (min_bbox[0] > node_min_bbox[0]) min_bbox[0] = node_min_bbox[0];
		if (min_bbox[1] > node_min_bbox[1]) min_bbox[1] = node_min_bbox[1];
		if (min_bbox[2] > node_min_bbox[2]) min_bbox[2] = node_min_bbox[2];
    }

	return (min_bbox);
}


VEC3 SceneGraphNode::getNodeOnlyWorldMaxCorner()
{
	VEC3 max_bbox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	if (getPolygonMesh().getVertexNumber())
	{
		// Process the node
		VEC3 node_max_bbox = getWorldTransformationMatrix() * getPolygonMesh().getLocalMaxCorner();

		if (max_bbox[0] < node_max_bbox[0]) max_bbox[0] = node_max_bbox[0];
		if (max_bbox[1] < node_max_bbox[1]) max_bbox[1] = node_max_bbox[1];
		if (max_bbox[2] < node_max_bbox[2]) max_bbox[2] = node_max_bbox[2];
	}

	return (max_bbox);
}


/*
VEC3 SceneGraphNode::getLocalMinCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMinCorner());
}


VEC3 SceneGraphNode::getLocalMaxCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMaxCorner());
}


VEC3 SceneGraphNode::getWorldMinCorner()
{
	return (getWorldTransformationMatrix() * getLocalMinCorner());
}


VEC3 SceneGraphNode::getWorldMaxCorner()
{
	return (getWorldTransformationMatrix() * getLocalMaxCorner());
}*/


const PhotonCrossSection& SceneGraphNode::getPhotonCrossSection() const
{
	return (getPolygonMesh().getPhotonCrossSection());
}


SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel)
{
    for (std::vector<SceneGraphNode*>::iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if ((*ite)->m_label == aLabel)
    	{
    		return **ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


const SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel) const
{
    for (std::vector<SceneGraphNode*>::const_iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if ((*ite)->m_label == aLabel)
    	{
    		return **ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


void SceneGraphNode::updateWorldMatrix()
{
	if 	(!m_world_properties_up_to_date)
	{
		m_global_transformation = getLocalTransformationMatrix();

		if (m_p_parent)
		{
			m_global_transformation = m_p_parent->getWorldTransformationMatrix() * m_global_transformation;
		}

		m_world_properties_up_to_date = true;
	}
}




/*
SceneGraphNode& SceneGraphNode::operator=(const SceneGraphNode& aNode)
{
	// The current node has children
	// They need to be deleted
	if (m_p_children.size())
	{
		// Add all the children of the node to the temp list
		std::vector<SceneGraphNode*> p_temp_pool_of_nodes;
		for (std::vector<SceneGraphNode>::iterator ite = m_p_children.begin();
				ite != m_p_children.end();
				++ite)
		{
			p_temp_pool_of_nodes.push_back(&(*ite));
		}

		// Process until the list is empty
		while (p_temp_pool_of_nodes.size())
		{

		}
	}

	return (*this);
}
*/
