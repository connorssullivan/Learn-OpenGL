#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

inline constexpr float DEFAULT_YAW = -90.0f;
inline constexpr float DEFAULT_PITCH = 0.0f;
inline constexpr float SPEED = 5.25f;
inline constexpr float SENSETIVITY = 0.1f;
inline constexpr float ZOOM = 45.0f;

class Camera
{
    public:
        enum class CameraMovement
        {
            FORWARD=0,
            BACKWARD,
            LEFT,
            RIGHT,
            INVALID_CAMERA_MOVE
        };

        // Camera Attributes
        glm::vec3 m_positions;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        // Eulur Angles
        float m_yaw;
        float m_pitch;

        // Camera Options
        float m_mouseSensetivity;
        float m_mouseSpeed;
        float m_zoom;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
                float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH)
                : m_front {glm::vec3(0.0f, 0.0f, -1.0f)}
                , m_mouseSpeed {SPEED}
                , m_mouseSensetivity {SENSETIVITY}
                , m_zoom {ZOOM}
        {
            m_positions = position;
            m_worldUp = up;
            m_yaw = yaw;
            m_pitch = pitch;
            UpdateCameraVectors();
        };

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, 
            float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH)
                : m_front {glm::vec3(0.0f, 0.0f, -1.0f)}
                , m_mouseSpeed {SPEED}
                , m_mouseSensetivity {SENSETIVITY}
                , m_zoom {ZOOM}
        {
            m_positions = {posX, posY, posZ};
            m_worldUp = {upX, upY, upZ};
            m_yaw = yaw;
            m_pitch = pitch;
            UpdateCameraVectors();
        };

        glm::mat4 GetViewMatrix()
        {
                                // Eye         // Center          // Up
            return glm::lookAt(m_positions, m_positions + m_front, m_up);
        };

        glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) 
        {
            // Calculate Camera Direction
            glm::vec3 zaxis = glm::normalize(position - target);

            // Calculate Camera X axis
            glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));

            // Calculate camera up vector 
            glm::vec3 yaxis = glm::cross(zaxis,  yaxis);

            // Create translation matrix 
            glm::mat4 translation = glm::mat4(1.0f);
            translation[3][0] = -position.x;
            translation[3][1] = -position.y;
            translation[3][2] = -position.z;

            // Create rotation matrix
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation[0][0] = xaxis.x; // First Row first Column
            rotation[1][0] = xaxis.y;
            rotation[2][0] = xaxis.z;

            rotation[0][1] = yaxis.x; // Second Column First Column
            rotation[1][1] = yaxis.y;
            rotation[2][1] = yaxis.z;

            rotation[0][2] = zaxis.x; // third Row First Column
            rotation[1][2] = zaxis.y;
            rotation[2][2] = zaxis.z;
            
            return rotation * translation;
        }

        void ProcessKeyboard(CameraMovement dir, float deltaTime)
        {
            float velocity = m_mouseSpeed * deltaTime;
            
            if (dir == CameraMovement::FORWARD)
                m_positions += m_front * velocity;
            if (dir == CameraMovement::BACKWARD)
                m_positions -= m_front * velocity;
            if (dir == CameraMovement::LEFT)
                m_positions -= m_right * velocity;
            if (dir == CameraMovement::RIGHT)
                m_positions += m_right * velocity;
            //m_positions.y = 0.0;

        }
        
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) 
        {
            xoffset *= m_mouseSensetivity;
            yoffset *= m_mouseSensetivity;

            m_yaw += xoffset;
            m_pitch += yoffset;

            if (constrainPitch)
            {
                if (m_pitch > 89.0f)
                    m_pitch = 89.0f;
                if (m_pitch < -89.0f)
                    m_pitch = -89.0f;
            }

            UpdateCameraVectors();
        }


        void ProcessMouseScroll(float yOffset)
        {
            m_zoom -= yOffset;

            if (m_zoom < -1.f)
                m_zoom = 1.f;
            if (m_zoom > 45.f)
                m_zoom = 45.f;
        }
        

        private:
            void UpdateCameraVectors()
            {
                // calculate the new Front vector
                glm::vec3 front;
                front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
                front.y = sin(glm::radians(m_pitch));
                front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
                m_front = glm::normalize(front);
                // also re-calculate the Right and Up vector
                m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
                m_up    = glm::normalize(glm::cross(m_right, m_front));
            }
};