#include "config.h"
#include "graphics/Circle.h"
#include "graphics/Shader.h"
#include "graphics/RoundedRectangle.h"
#include "graphics/Rectangle.h"
#include "graphics/AltitudeLines.h"


// Function to draw the white line passing through the center of the circle
void drawLine(float x1, float y1, float x2, float y2, Shader &shader)
{
    // Create vertices for the line
    float vertices[] = {
        x1, y1,
        x2, y2};

    // Create VAO and VBO for the line
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color to white
    glBindVertexArray(VAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1.0f);
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// -----------------------------------------------------------------------
// Function to process input and update roll and pitch angles

float rollAngle = 0.0f;       // Roll angle in degrees
float pitchAngle = 0.0f;      // Pitch angle in degrees
bool leftKeyPressed = false;  // Tracks if the left key is pressed
bool rightKeyPressed = false; // Tracks if the right key is pressed
bool upKeyPressed = false;    // Tracks if the up key is pressed
bool downKeyPressed = false;  // Tracks if the down key is pressed

void processInput(GLFWwindow *window, float &rollAngle, float &pitchAngle)
{
    // Handle roll (left/right keys)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (!leftKeyPressed)
        {
            rollAngle += 1.0f;
            if (rollAngle > 90.0f)
                rollAngle = 90.0f;
            leftKeyPressed = true;
        }
    }
    else
    {
        leftKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (!rightKeyPressed)
        {
            rollAngle -= 1.0f;
            if (rollAngle < -90.0f)
                rollAngle = -90.0f;
            rightKeyPressed = true;
        }
    }
    else
    {
        rightKeyPressed = false;
    }

    // Handle pitch (up/down keys)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (!upKeyPressed)
        {
            pitchAngle -= 1.0f; // Move up by 1 degree
            if (pitchAngle < -20.0f)
                pitchAngle = -20.0f; // Clamp to 90 degrees
            upKeyPressed = true;
        }
    }
    else
    {
        upKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (!downKeyPressed)
        {
            pitchAngle += 1.0f; // Move down by 1 degree
            if (pitchAngle > 20.0f)
                pitchAngle = 20.0f; // Clamp to -90 degrees
            downKeyPressed = true;
        }
    }
    else
    {
        downKeyPressed = false;
    }
}

// ----------------------------------------------------------------------- 

// Main function

int main()
{
    // Initializing our window
    GLFWwindow *window;
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Here we are creating the window by passing it the width, height, and the title
    window = glfwCreateWindow(985, 985, "Test Attitude Indicator", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);  // Load OpenGL functions using glad

    // Creating the background Rounded Rectangle
    Rectangle background(0.0f, 0.0f, 1.5f, 1.5f);
    Rectangle lowerHalf(0.0f, -0.6f, 0.6f, 1.2f);

    // Creating our line and text shaders seperately
    Shader shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");
    Shader textShader("../src/shaders/text_vertex.txt", "../src/shaders/text_fragment.txt");

    // Setting background color for the window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Creating the different elements to be shown in the window (particularly all the circles in the window)
    Circle mainCircle(0.0f, 0.0f, 0.6f, 360);
    Circle outermostCircle(0.0f, 0.0f, 0.7f, 360);
    Circle innerOutlineCircle(0.0f, 0.0f, 0.65f, 360);
    Circle crossHairDost(0.0f, 0.0f, 0.01f, 360);

    // Creating quarter circles for our bottom halkf
    float quarterCircleRadius = 0.3f;

    QuarterCircle bottomLeftQuarter(-0.3f, 0.0f, 1.2f, 90, M_PI, 1.5f * M_PI);        // Bottom-left
    QuarterCircle bottomRightQuarter(0.3f, 0.0f, 1.2f, 90, 1.5f * M_PI, 2.0f * M_PI); // Bottom-right

    // Creating the altitude lines
    AltitudeLines altitudeLines(-0.2f, 0.2f, 0.05f, 0.2f, 0.004f);

    // Error check for the window creation
    if (!window)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Set the viewport to match the window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window, rollAngle, pitchAngle);
        // -------------------------------------------------
        // Setting up the shader and transforming the elements
        shader.use();
        glm::mat4 transform = glm::mat4(1.0f);
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // -------------------------------------------------
        // Drawing all cosmetic elements

        // Draw the background rectangle
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.4118f, 0.4078f, 0.4118f, 1.0f))); // Grey color
        background.draw();

        // Draw the outermost circle
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.502f, 0.502f, 0.502f, 1.0f))); // Dark grey
        outermostCircle.draw();

        // Draw the outline of the outermost circle
        glLineWidth(4.0f);
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // White color
        outermostCircle.drawOutline();

        // Draw the inner outline circle
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.4118f, 0.4078f, 0.4118f, 1.0f))); // Light grey
        innerOutlineCircle.draw();

        // Draw the outline of the inner outline circle
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // Dark grey
        glLineWidth(1.0f);
        innerOutlineCircle.drawOutline();

        // -------------------------------------------------
        // This contains the main inner circle logic
        // Rotate the main circle based on the roll angle
        transform = glm::mat4(1.0f);                                                              // Reset to identity matrix
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));


        // Enable stencil test to ensure the rectangle is only drawn inside the circle
        glEnable(GL_STENCIL_TEST);

        // Configure the stencil buffer to mark the blue circle area
        glClear(GL_STENCIL_BUFFER_BIT);     
        glStencilFunc(GL_ALWAYS, 1, 0xFF);         
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);                       

        // Draw the blue circle (this marks the stencil buffer) (This is the main blue background circle)
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.2471f, 0.6392f, 0.9765f, 1.0f))); // Blue color
        mainCircle.draw();

        // Configure the stencil test to only allow drawing inside the circle
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);

        // Draw the rectangle (only the part inside the circle will be visible)
        // Add pitch as a vertical translation
        transform = glm::translate(transform, glm::vec3(0.0f, pitchAngle * 0.01f, 0.0f));
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.7216f, 0.5255f, 0.0431f, 1.0f))); // Brown color
        lowerHalf.draw();

        // Resetting the transformation matrix
        transform = glm::mat4(1.0f); // Identity matrix
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // Set the color for the quarter circles
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.7216f, 0.5255f, 0.0431f, 1.0f))); // Brown color

        // Draw the bottom-left quarter circle
        bottomLeftQuarter.draw();

        // Draw the bottom-right quarter circle
        bottomRightQuarter.draw();

        // Disable the stencil test
        glDisable(GL_STENCIL_TEST);

        // -------------------------------------------------

        // Apply pitch as a vertical translation
        // Reset the transformation matrix
        transform = glm::mat4(1.0f);                                      // Identity matrix
        float pitchX = pitchAngle * 0.01f * sin(glm::radians(rollAngle)); // X-component of pitch
        float pitchY = pitchAngle * 0.01f * cos(glm::radians(rollAngle)); // Y-component of pitch

        // Apply the dynamic translation (This is to tackle the pitch angle when drawing the altitude lines)
        transform = glm::translate(transform, glm::vec3(-pitchX, pitchY, 0.0f));

        // Apply roll (rotation around the Z-axis)
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        // Set the combined transformation matrix in the shader
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        altitudeLines.draw(shader, textShader, rollAngle, pitchAngle); // Draw the altitude lines

        // Reset the transformation matrix to only considering the roll angle
        transform = glm::mat4(1.0f); // Identity matrix
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // ----------------------------------------
        // Draw the translucent overlay
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Transformation matrix for the overlay to match the roll angle
        transform = glm::mat4(1.0f); // Identity matrix
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // Draw the inward translucent overlay
        mainCircle.drawInwardOverlay(0.075f, glm::vec4(1.0f, 1.0f, 1.0f, 0.3f), shader); // Thickness = 0.05, Black with 50% transparency
        glLineWidth(10.0f);
        mainCircle.drawOverlayLines(0.6f, 12, shader, 0.075f); // 12 lines, White with 50% transparency
        glLineWidth(1.0f);
        // Disable blending after drawing the overlay
        glDisable(GL_BLEND);

        // -------------------------------------
        // Reseting the transformation matrix
        transform = glm::mat4(1.0f); // Identity matrix
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // Transformation matrix for the center white line to match the roll angle
        transform = glm::mat4(1.0f); // Identity matrix
        transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // Draw the white line passing through the center of the circle
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // Black color
        drawLine(-0.6f, 0.0f, 0.6f, 0.0f, shader);

        // Resetting the transformation matrix
        transform = glm::mat4(1.0f); // Identity matrix
        shader.setUniformMat4("uTransform", glm::value_ptr(transform));

        // Draw the top triangle that is the yellow indicator
        mainCircle.drawTopTriangle(0.6f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), shader, 0.05f); // Yellow triangle

        // Draw the fork
        mainCircle.drawFork(0.6f, glm::vec4(0.6471f, 0.1647f, 0.1647f, 1.0f), shader, 0.075f);

        // Draw the crosshair dot
        shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); // Black color
        crossHairDost.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
