//
//  main.cpp
//  triangle
//
//  Created by Nazirul Hasan on 26/8/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos.x,aPos.y,0.0, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //lines, line_strip, line_loop, triangles, triangle_strip
    float lvertices[] = {
0.305948929005705,0.169918623140959,
0.65633398808967,0.173787921304523,
0.656377638512144,0.213902659557883,
0.656424406821937,0.256882736257911,
0.660823745829825,0.299875284507217,
0.667393134412122,0.337143391637826,
0.673971876656377,0.383007514108439,
0.679443768902191,0.411676488011722,
0.684903189598728,0.428884108128332,
0.697992080566208,0.457574907242852,
0.709977239422567,0.471935896236709,
0.723056776728089,0.492030680011224,
0.733956910797243,0.509253889564431,
0.747023976553487,0.517887319552271,
0.764437377233186,0.520802544196052,
0.784023945374614,0.520858666167804,
0.80034296760515,0.518040096030929,
0.817737660960933,0.503763289994699,
0.831864808405824,0.486611791849842,
0.8470738627506,0.463732734698967,
0.859015371184485,0.437978985439466,
0.872038786518255,0.406497677173946,
0.880697159604651,0.363542543572475,
0.88718548311664,0.326311851089701,
0.892585663954104,0.289078040719608,
0.896903937891684,0.257571789355532,
0.89686652324385,0.22318772799551,
0.899002276057743,0.185944563963458,
0.898992922395784,0.177348548623452,
0.916396969413525,0.171667757927228,
0.990393789168459,0.174745112711626,
0.990537211985159,0.306550681258379,
0.988445109593739,0.38390858354379,
0.992835094939669,0.418305116453091,
0.996124466061796,0.441237177688398,
0.998338166058678,0.475627474823059,
0.99728120225735,0.504277741402425,
0.99296604620709,0.538649331213169,
0.987547158045708,0.558691110903251,
0.986540080441493,0.633186792629314,
0.982212452841954,0.656097028653384,
0.929180307423689,0.919555389268231,
0.923767655036947,0.94532784585165,
0.91399307829015,0.962491815545786,
0.902029744645028,0.968188195678608,
0.894415863810681,0.971031708914039,
0.512477785052848,0.969937330464876,
0.499420072958563,0.969899915817042,
0.485274218189754,0.969859383281888,
0.473298412995354,0.964094409628035,
0.462410750475477,0.958332553861503,
0.448264895706669,0.958292021326349,
0.439547282761201,0.946805724441118,
0.429741527141209,0.935316309668568,
0.419932653633897,0.920961556449349,
0.409038755339382,0.90946902378948,
0.39923299971939,0.89797960901693,
0.392688554235649,0.88363420945967,
0.380690923830012,0.857811866679138,
0.370869578773423,0.831995759673245,
0.359963208929629,0.809041873226701,
0.354491316683815,0.780372899323418,
0.343575593178062,0.748822997536868,
0.333744894459514,0.71441087519097,
0.327187977426495,0.688604121847036,
0.317369750257225,0.665653353287812,
0.308649019424438,0.651301717955912,
0.296673214230037,0.64553674430206,
0.28251800579927,0.636900196426901,
0.271627225392074,0.628273002213699,
0.257481370623265,0.628232469678545,
0.205250522246126,0.628082811087207,
0.194369095500888,0.628051632214011,
0.185657718330059,0.622296012222118,
0.0789667321423003,0.573279705671436,
0.0691609765223084,0.561790290898886,
0.0550026502042216,0.550288404577058,
0.0419293486733389,0.535924297695881,
0.0332117357278708,0.52443800081065,
0.0266610544694914,0.504361924360053,
0.0233779191220029,0.487160540018083,
0.0211953979983163,0.481423627350107,
0.0146384809652979,0.455616874006173,
0.0102453777320487,0.418355002650204,
0.0102079630842141,0.383970941290181,
0.0101830199856577,0.361048233716833,
0.00905122688866024,0.320930377576154,
0.00890156829732173,0.183394132136064,
0.0763632962304742,0.180722102703208,
0.0785801141146759,0.217977738284538,
0.0808000498861971,0.258098712312536,
0.0830137498830792,0.292489009447198,
0.088488760016213,0.32402332179715,
0.0950425591619119,0.346964736694415,
0.10159947619493,0.372771490038349,
0.111427057026159,0.404318273937579,
0.123421569544476,0.427275278271443,
0.136507342624637,0.453100738939294,
0.150678140492002,0.476063979047797,
0.16810089483366,0.487575219031584,
0.185523649175318,0.49908645901537,
0.200757646618651,0.499130109437844,
0.213806005050977,0.490571508745673,
0.226854363483303,0.482012908053502,
0.240978393040875,0.461996071461977,
0.252935490911358,0.450569014435818,
0.267050166806971,0.421956162504286,
0.277897296791694,0.390468618464128,
0.285473762978206,0.353241043868674,
0.293043993390078,0.310282792379883,
0.297356031553019,0.27304586412247,
0.300586162816075,0.241536494871075,
0.30598634365354,0.204302684500982
    };
    float vertices[] = {
 0.096037165216849,0.261007701181679,
0.100424032675459,0.292538895644311,
0.106984067595797,0.321210987434914,
0.113540984628815,0.347017740778848,
0.121192280110996,0.378558288903438,
0.143001901911264,0.421600723349858,
0.156075203442147,0.435964830231035,
0.16914538708571,0.447463598665544,
0.183294359741838,0.450369469647366,
0.194175786487076,0.450400648520562,
0.206145355906837,0.450434945281077,
0.21810557166464,0.441873226701586,
0.230059551647803,0.427580831228759,
0.243098556418171,0.410426215196582,
0.252866897390328,0.38753156860911,
0.262641474137124,0.370367598914975,
0.268054126523867,0.344595142331555,
0.275636828485018,0.313098244629438,
0.278870077635394,0.284454213824712,
0.283185233685654,0.250082624013967,
0.284235961712343,0.215701680541265,
0.28419231128987,0.175586942287905,
0.283063636080192,0.138334424593895,
0.279768029183425,0.10967168646525,
0.275384279612134,0.0810058304492874,
0.268833598353755,0.0609297539986902,
0.264456084557104,0.0379945748760638,
0.258996663860568,0.0207869547594548,
0.253543478938671,0.00931001153618282,
0.244816512331244,-0.0107723006890532,
0.230655068125837,-0.0251395254575501,
0.221937455180369,-0.0366258223427806,
0.208870389424126,-0.045259252330621,
0.196903937891684,-0.0424282106444675,
0.181669940448352,-0.0424718610669412,
0.166442178779658,-0.0367848345960777,
0.154481963021856,-0.0282231160165875,
0.14470115050042,-0.0167898232157892,
0.136011598540828,-0.00248807408100293,
0.127325164468556,0.0146790135004517,
0.122991301094378,0.0318585726311848,
0.114304867022105,0.0490256602126397,
0.108892214635363,0.0747981167960588,
0.104558351261185,0.0919776759267919,
0.100230723661646,0.114887911950861,
0.0969943566239516,0.140666604308919,
0.0959529822592211,0.183643563121628,
0.0959872790197362,0.215162286034982,
0.0960091042309731,0.235219655161662
    };
    float vertices3[] = {
    0.756265647452706, 0.8745833518510289,
0.7718738426440307, 0.5893960268432514,
0.942355154585722, 0.5905959735122883,
0.9455401982134128, 0.6013066085951736,
0.9465149697050502, 0.614817119239145,
0.9463964564537873, 0.6283720723523399,
0.9451609558093715, 0.6446824585573976,
0.9417151830289024, 0.6637927203235412,
0.8964431210464721, 0.8417848095640194,
0.8897886019880598, 0.8528954268699169,
0.8853680577159534, 0.8584951779920893,
0.8820170955364948, 0.866761477267677,
0.8765269691717407, 0.8696946802364339,
0.8688265706709332, 0.8754277587662771,
0.864453431699332, 0.8756055286431714,
0.8600802927277307, 0.8757832985200658

    };
    float vertices2[] = {
    0.5681732663733464, 0.8876494378027643,
0.5681732663733464, 0.600284431803031,
0.721535339170111, 0.5968623616728146,
0.7015303023569323, 0.8849384471801254
    };
    float vertices1[] =
    { 0.527721730886034,0.889293809164037,
0.530234211812808,0.601928803164303,
0.3203472438262,0.607750766632594,
0.326598817830318,0.642726989911559,
0.332897797134942,0.672281231945246,
0.361539487133905,0.771387938313852,
0.368955453831681,0.798186747255677,
0.375349243737315,0.816897026798808,
0.381790438943454,0.830185325096662,
0.389348621542746,0.840718190302653,
0.398000088884938,0.85120661303942,
0.40665155622713,0.861695035776187,
0.415303023569322,0.872183458512955,
0.42400189621202,0.877249900004444,
0.433794053597617,0.88227189902671,
0.439213072011614,0.88747166792587,
0.449028932047464,0.889782676325496,
0.459914374175962,0.894760232878538,
0.470823518954713,0.897026798808941,
0.480663081640815,0.896626816585929,
0.488316074841118,0.896315719301364,
0.49487578329852,0.896049064486022,
0.528791312978682,0.891960357317452


    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lvertices), lvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);



    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindVertexArray(VAO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindVertexArray(VAO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float centerX = 0.0f;
        float centerY = 0.0f;
        for (int i = 0; i < sizeof(lvertices) / (2 * sizeof(float)); i += 2) {
            centerX += lvertices[i];
            centerY += lvertices[i + 1];
        }
        centerX /= (sizeof(lvertices) / (2 * sizeof(float)));
        centerY /= (sizeof(lvertices) / (2 * sizeof(float)));

        // create transformations
        glm::mat4 trans = glm::mat4(1.0f); // initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(-centerX, -centerY, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(scale_X, scale_Y, 1.0));


        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // create transformations
        /*glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));*/

        //modelMatrix = rotationMatrix * scaleMatrix;

        // get matrix's uniform location and set matrix
       /*  glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        translationMatrix = glm::translate(identityMatrix, glm::vec3(-0.6f, -0.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
        modelMatrix = translationMatrix  * scaleMatrix;  */
        glUseProgram(shaderProgram);
        /* unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
         // draw our first triangle */
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_STRIP, 0, sizeof(lvertices) / (2 * sizeof(float)));
        glDrawArrays(GL_LINE_LOOP, 0, 113);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        /*glDrawArrays(GL_TRIANGLE_FAN, 0, segments); */
        glBindVertexArray(0); // no need to unbind it every time
        glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_STRIP, 0, sizeof(lvertices) / (2 * sizeof(float)));
        glDrawArrays(GL_LINE_LOOP, 0, 49);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        glBindVertexArray(0); // no need to unbind it every time
        glm::vec3 translationVector = glm::vec3(0.2f, -0.27f, 0.0f);
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, translationVector);
        glUseProgram(shaderProgram);
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(VAO2);
        glDrawArrays(GL_LINE_LOOP, 0, 49);
        glBindVertexArray(0);
        // create transformations
        trans = glm::mat4(1.0f); // initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(-0.35, -0.25, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(scale_X, scale_Y, 1.0));
        glUseProgram(shaderProgram);
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO3);
        glDrawArrays(GL_LINE_LOOP, 0, 23);
        glBindVertexArray(0);

        trans = glm::mat4(1.0f); // initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(-0.35, -0.25, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(scale_X, scale_Y, 1.0));
        glUseProgram(shaderProgram);
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(VAO4);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);

        trans = glm::mat4(1.0f); // initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(-0.37, -0.24, 0.0f));
        trans = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(scale_X, scale_Y, 1.0));
        glUseProgram(shaderProgram);
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        //int colorLocation = glGetUniformLocation(shaderProgram, "colorInfo");
        //glUniform4fv(colorLocation, 1, glm::value_ptr(glm::vec4(1.0, 0.0, 0.0, 1.0)));

        glBindVertexArray(VAO5);
        glDrawArrays(GL_LINE_LOOP, 0, 16);
        glBindVertexArray(0);
        /* translationMatrix = glm::translate(identityMatrix, glm::vec3(0.6f, -0.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
        modelMatrix = translationMatrix * scaleMatrix;
        glUseProgram(shaderProgram);
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments); */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
