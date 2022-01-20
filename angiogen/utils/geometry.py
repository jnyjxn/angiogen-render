import math

def generate_equidistant_points(n_points):
	a = 4*math.pi/n_points
	d = math.sqrt(a)

	M_theta = round(math.pi/d)
	d_theta = math.pi / M_theta
	d_phi = a/d_theta

	for m in range(M_theta - 1):
		theta = math.pi*(m + 0.5)/M_theta
		M_phi = round(2*math.pi*math.sin(theta)/d_phi)
		for n in range(M_phi - 1):
			phi = 2*math.pi*(n + 0.5)/M_phi
			theta_d = (theta / math.pi) * 180
			phi_d = (phi / math.pi) * 180
			yield [180 - theta_d, phi_d]