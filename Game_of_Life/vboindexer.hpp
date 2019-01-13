#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP
/** A straightforward store of rules to govern a simulation.
	@author http://www.opengl-tutorial.org/
*/
void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals
);

#endif