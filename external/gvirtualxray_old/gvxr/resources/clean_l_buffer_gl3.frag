/**
********************************************************************************
*
*   @file       clean_l_buffer_gl3.frag
*
*   @brief      Adaptive filtering to clean the L-buffer.
*
*   @version    1.0
*
*   @date       30/12/2013
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//  Input
//******************************************************************************
smooth in vec3 g_texture_coordinates;


//******************************************************************************
//  Output
//******************************************************************************
out vec3 fragColor;


//******************************************************************************
//  Defines
//******************************************************************************
#define KERNEL_SIZE_3x3  8
#define KERNEL_SIZE_5x5 16
#define KERNEL_SIZE_7x7 24
#define EPSILON         0.5


//******************************************************************************
//  Uniform variables
//******************************************************************************
uniform sampler2D g_l_buffer_texture;
uniform float     g_x_offset;
uniform float     g_y_offset;
uniform float     g_cutoff;


//******************************************************************************
//  Global variables
//******************************************************************************
vec2 g_offset_3x3[KERNEL_SIZE_3x3];
vec2 g_offset_5x5[KERNEL_SIZE_5x5];
vec2 g_offset_7x7[KERNEL_SIZE_7x7];


//-----------------------------
bool isValid(vec3 lBufferValue)
//-----------------------------
{
    // Return true if lBufferValue is zero, false if it is smaller or greater than zero
    // Care is given to handle floating point numbers' inacuracy
    return ((abs(lBufferValue.g) < EPSILON) && (0.0 <= lBufferValue.r) && (lBufferValue.r <= g_cutoff));
}


//---------------------------------
vec3 filter7x7(vec3 anInitialValue)
//---------------------------------
{
    int i = 0;
    float count = 0.0;
    vec3 value = vec3(0.0, 0.0, 0.0);
    vec2 tex_coord;
    vec3 l_buffer_value;

    // Process all the texels of the neighbourhood
    for (i = 0; i < KERNEL_SIZE_7x7; ++i)
    {
        // Get the texel value
        tex_coord = vec2(g_texture_coordinates.st + g_offset_7x7[i]);

        // Get the L-buffer value
        l_buffer_value = texture(g_l_buffer_texture, tex_coord).rgb;

        // The L-buffer value is valid
        if (isValid(l_buffer_value))
        {
            // Update the cumulated value
            value += l_buffer_value;
        
            // Update the counter
            count += 1.0;
        }
    }

    // At least one valid value has been found in the neighbourhood
    if (count > EPSILON)
    {
        // Return the average value
        value /= count;
    }

    // Return the filtered value
    return (value);
}


//---------------------------------
vec3 filter5x5(vec3 anInitialValue)
//---------------------------------
{
    int i = 0;
    float count = 0.0;
    vec3 value = vec3(0.0, 0.0, 0.0);
    vec2 tex_coord;
    vec3 l_buffer_value;
    
    // Process all the texels of the neighbourhood
    for (i = 0; i < KERNEL_SIZE_5x5; ++i)
    {
        // Get the texel value
        tex_coord = vec2(g_texture_coordinates.st + g_offset_5x5[i]);

        // Get the L-buffer value
        l_buffer_value = texture(g_l_buffer_texture, tex_coord).rgb;
    
        // The L-buffer value is valid
        if (isValid(l_buffer_value))
        {
            // Update the cumulated value
            value += l_buffer_value;
        
            // Update the counter
            count += 1.0;
        }
    }
    
    // No valid valid value has been found in the neighbourhood
    if (count <= EPSILON)
    {
        // Filter more
        value = filter7x7(anInitialValue);
    }
    else
    {
        // Return the average value
        value /= count;
    }

    // Return the filtered value
    return (value);
}


//---------------------------------
vec3 filter3x3(vec3 anInitialValue)
//---------------------------------
{
    int i = 0;
    float count = 0.0;
    vec3 value = vec3(0.0, 0.0, 0.0);
    vec2 tex_coord;
    vec3 l_buffer_value;

    // Process all the texels of the neighbourhood
    for (i = 0; i < KERNEL_SIZE_3x3; ++i)
    {
        // Get the texel value
        tex_coord = vec2(g_texture_coordinates.st + g_offset_3x3[i]);

        // Get the L-buffer value
        l_buffer_value = texture(g_l_buffer_texture, tex_coord).rgb;
    
        // The L-buffer value is valid
        if (isValid(l_buffer_value))
        {
            // Update the cumulated value
            value += l_buffer_value;
        
            // Update the counter
            count += 1.0;
        }
    }
    
    // No valid valid value has been found in the neighbourhood
    if (count <= EPSILON)
    {
        // Filter more
        value = filter5x5(anInitialValue);
    }
    else
    {
        // Return the average value
        value /= count;
    }

    // Return the filtered value
    return (value);
}


//-------------------------------
bool isHomogeneous(float aValue1,
                   float aValue2,
                   float aValue3,
                   float aValue4,
                   float aValue5)
//-------------------------------
{
    float average = 0.0;
    int counter = 0;
    
    if (aValue2 > 0.01)
    {
        average += aValue2;
        ++counter;
    }
    
    if (aValue3 > 0.01)
    {
        average += aValue3;
        ++counter;
    }
    
    if (aValue4 > 0.01)
    {
        average += aValue4;
        ++counter;
    }
    
    if (aValue5 > 0.01)
    {
        average += aValue5;
        ++counter;
    }
    
    if (counter == 0)
    {
        return true;
    }

    average /= float(counter);

    if (abs(aValue1 - average) > 0.25 * average) return false;
    
    return true;
}


//-------------
void main(void)
//-------------
{
    // Set the texture coordinate set of the 3x3 kernel
    g_offset_3x3[0] = vec2( 0.0, -g_y_offset);
    g_offset_3x3[1] = vec2(-g_x_offset,  0.0);
    g_offset_3x3[2] = vec2( g_x_offset,  0.0);
    g_offset_3x3[3] = vec2( 0.0,  g_y_offset);
    g_offset_3x3[4] = vec2( g_x_offset,  g_y_offset);
    g_offset_3x3[5] = vec2(-g_x_offset,  g_y_offset);
    g_offset_3x3[6] = vec2(-g_x_offset, -g_y_offset);
    g_offset_3x3[7] = vec2( g_x_offset, -g_y_offset);

    // Set the texture coordinate set of the 5x5 kernel
    g_offset_5x5[ 0] = vec2(-2.0 * g_x_offset, -2.0 * g_y_offset);
    g_offset_5x5[ 1] = vec2(-g_x_offset, -2.0 * g_y_offset);
    g_offset_5x5[ 2] = vec2( 0.0, -2.0 * g_y_offset);
    g_offset_5x5[ 3] = vec2( g_x_offset, -2.0 * g_y_offset);
    g_offset_5x5[ 4] = vec2( 2.0 * g_x_offset, -2.0 * g_y_offset);
    g_offset_5x5[ 5] = vec2(-2.0 * g_x_offset,  2.0 * g_y_offset);
    g_offset_5x5[ 6] = vec2(-g_x_offset,  2.0 * g_y_offset);
    g_offset_5x5[ 7] = vec2( 0.0,  2.0 * g_y_offset);
    g_offset_5x5[ 8] = vec2( g_x_offset,  2.0 * g_y_offset);
    g_offset_5x5[ 9] = vec2( 2.0 * g_x_offset,  2.0 * g_y_offset);
    g_offset_5x5[10] = vec2(-2.0 * g_x_offset, -g_y_offset);
    g_offset_5x5[11] = vec2( 2.0 * g_x_offset, -g_y_offset);
    g_offset_5x5[12] = vec2(-2.0 * g_x_offset,  0.0);
    g_offset_5x5[13] = vec2( 2.0 * g_x_offset,  0.0);
    g_offset_5x5[14] = vec2(-2.0 * g_x_offset,  g_y_offset);
    g_offset_5x5[15] = vec2( 2.0 * g_x_offset,  g_y_offset);

    // Set the texture coordinate set of the 7x7 kernel
    g_offset_7x7[ 0] = vec2(-3.0 * g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 1] = vec2(-2.0 * g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 2] = vec2(-g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 3] = vec2( 0.0, -3.0 * g_y_offset);
    g_offset_7x7[ 4] = vec2( g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 5] = vec2( 2.0 * g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 6] = vec2( 3.0 * g_x_offset, -3.0 * g_y_offset);
    g_offset_7x7[ 7] = vec2(-3.0 * g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[ 8] = vec2(-2.0 * g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[ 9] = vec2(-g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[10] = vec2( 0.0,  3.0 * g_y_offset);
    g_offset_7x7[11] = vec2( g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[12] = vec2( 2.0 * g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[13] = vec2( 3.0 * g_x_offset,  3.0 * g_y_offset);
    g_offset_7x7[14] = vec2(-3.0 * g_x_offset, -2.0 * g_y_offset);
    g_offset_7x7[15] = vec2( 3.0 * g_x_offset, -2.0 * g_y_offset);
    g_offset_7x7[16] = vec2(-3.0 * g_x_offset, -g_y_offset);
    g_offset_7x7[17] = vec2( 3.0 * g_x_offset, -g_y_offset);
    g_offset_7x7[18] = vec2(-3.0 * g_x_offset,  0.0);
    g_offset_7x7[19] = vec2( 3.0 * g_x_offset,  0.0);
    g_offset_7x7[20] = vec2(-3.0 * g_x_offset,  g_y_offset);
    g_offset_7x7[21] = vec2( 3.0 * g_x_offset,  g_y_offset);
    g_offset_7x7[22] = vec2(-3.0 * g_x_offset,  2.0 * g_y_offset);
    g_offset_7x7[23] = vec2( 3.0 * g_x_offset,  2.0 * g_y_offset);

    // Get the texel value
    vec3 texel_value = texture(g_l_buffer_texture, g_texture_coordinates.st).rgb;
    
    // The texel value is not valid
    if (!isValid(texel_value) || 
        !isHomogeneous(texel_value.r, 
         texture(g_l_buffer_texture, g_texture_coordinates.st + g_offset_3x3[0]).r,
         texture(g_l_buffer_texture, g_texture_coordinates.st + g_offset_3x3[1]).r,
         texture(g_l_buffer_texture, g_texture_coordinates.st + g_offset_3x3[2]).r,
         texture(g_l_buffer_texture, g_texture_coordinates.st + g_offset_3x3[3]).r))
    {
        // Filter the artefact
        texel_value = filter3x3(texel_value);
    }

    fragColor = texel_value;
}

