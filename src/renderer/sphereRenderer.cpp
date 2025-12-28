#include <iostream>
#include <math.h>

#include "particles/sphereBody.h"
#include "renderer/sphereRenderer.h"
#include "renderer/triangleMesh.h"

#define PI 3.14159265

sphereRenderer::sphereRenderer(const sphereBody& body, int stacks, int points, glm::vec4 colour) : body(body), colour(colour)
{
    buildMesh(stacks, points);
}

void sphereRenderer::buildMesh(int stacks, int points) 
{
    std::vector<glm::vec3> total_vertices((stacks + 1) * points);

    for (int stack = 0; stack <= stacks; stack++) {
        glm::vec3 stack_center = body.position;
        stack_center.y = body.position.y - (body.radius * ((stacks / 2) - stack) / (stacks / 2));

        double new_radius = sqrt(pow(body.radius, 2) - pow((body.position.y - stack_center.y), 2));
        double angle = (2 * PI) / points;

        for (int point = 0; point < points; point++) {
            glm::vec3 vertice = { stack_center.x + (new_radius * sin(angle * (point + 1))), stack_center.y, stack_center.z + (new_radius * cos(angle * (point + 1))) };
            total_vertices[(stack * points) + point] = vertice;
        }
        std::cout << " " << std::endl;
    }

    std::vector<Vertex> stack_vertices(total_vertices.size());

    std::vector<GLuint> stack_indices(total_vertices.size() * 6);

    int indx = 0;
    for (int stack = 0; stack < stacks; stack++) { // this ones gotta be less than final one so no overflow into nothing dont change it
        for (int point = 0; point < points; point++) {
            stack_vertices[(stack * points) + point] = Vertex{ total_vertices[(stack * points) + point], colour };
            if (not(stack == 0 or stack == stacks - 1)) {
                stack_indices[indx] = (stack * points) + point;
                stack_indices[indx + 1] = ((stack + 1) * points) + point;
                stack_indices[indx + 4] = ((stack + 1) * points) + point;
                if (point == points - 1) {
                    stack_indices[indx + 2] = (stack * points) + point + 1 - points;
                    stack_indices[indx + 3] = (stack * points) + point + 1 - points;
                    stack_indices[indx + 5] = ((stack + 1) * points) + point + 1 - points;
                }
                else {
                    stack_indices[indx + 2] = (stack * points) + point + 1;
                    stack_indices[indx + 3] = (stack * points) + point + 1;
                    stack_indices[indx + 5] = ((stack + 1) * points) + point + 1;
                }

                indx += 6;
            }
            else {
                if (stack == 0) { // first one
                    stack_indices[indx] = (stack * points) + point;
                    stack_indices[indx + 1] = ((stack + 1) * points) + point;

                    if (point != points - 1) {
                        stack_indices[indx + 2] = ((stack + 1) * points) + point + 1;
                    }
                    else {
                        stack_indices[indx + 2] = ((stack + 1) * points) + point + 1 - points;
                    }

                    indx += 3;
                }
                else if (stack == stacks - 1) { // last one
                    stack_indices[indx] = (stack * points) + point;
                    stack_indices[indx + 1] = ((stack + 1) * points); // + point not rly needed i think since its the same point, later make it just one

                    if (point != points - 1) {
                        stack_indices[indx + 2] = ((stack)*points) + point + 1;
                    }
                    else {
                        stack_indices[indx + 2] = ((stack)*points) + point + 1 - points;
                    }
                    indx += 3;
                }
            }
        }
        colour.r += (1.0f / (stacks));
        colour.g += (1.0f / (stacks));
        colour.b += (1.0f / (stacks));
    }
    stack_vertices[(stacks * points)] = Vertex{ total_vertices[(stacks * points) + 1] , colour };


    stack_indices.resize(indx);

    mesh = triangleMesh(stack_vertices, stack_indices);
}

void sphereRenderer::draw()
{
    int indexCount = mesh.bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}