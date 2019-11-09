#include <GL/glew.h> //beacause of some cryptic shit this motherfucker has to be the first
#include <core/Renderer.hpp>

#include <core/gl_extensions.hpp>
#include <GL/freeglut.h>

#include <deque>

namespace rcbe::core::rendering
{
class RendererImplementation
{
public:
    using handle_type = int;
public:
    RendererImplementation() = delete;
    RendererImplementation(RendererConfig &&config);

    RendererImplementation(const RendererImplementation &other) = delete;
    RendererImplementation &operator=(const RendererImplementation &other) = delete;

    RendererImplementation(RendererImplementation &&other) = delete;
    RendererImplementation &operator=(RendererImplementation &&other) = delete;

    ~RendererImplementation();

    void initGL();
    void initGLUT();
    void initGLEW();
    void initLights();
    const RendererConfig &config() const;
    void startGLUTMainLoop();

    // TODO: avoid copying
    void addObjectToBuffer(geometry::Mesh &&object);
    template <typename InputIterator>
    void addObjectsToBuffer(InputIterator begin, InputIterator end)
    {
        assert(begin != end);
        auto amount = std::distance(end, begin);
        _rendering_buffer.reserve(_rendering_buffer.size() + amount);
        _rendering_buffer.insert(_rendering_buffer.end(), begin, end);
    }

    void clearObjectBuffer();

private:

    static void renderingRoutine();
    static void reshape(int width, int height);
    static void drawMesh(const geometry::Mesh &mesh, const RendererConfig &config);
    static void timer(int millisec);

private:
    RendererConfig _config;
    handle_type _window_handle;

    // TODO: change Mesh -> RenderingObject
    static std::vector<geometry::Mesh> _rendering_buffer;
    static std::vector<GLuint> _vertex_buffers;
    static std::vector<GLuint> _inbex_buffer;
    static bool _vbo_supported;
    static int _width;
    static int _height;
    static double _zoom;
};

std::vector<geometry::Mesh> RendererImplementation::_rendering_buffer = {};
std::vector<GLuint> RendererImplementation::_vertex_buffers = {};
std::vector<GLuint> RendererImplementation::_inbex_buffer = {};
bool RendererImplementation::_vbo_supported = false;
int RendererImplementation::_width = 800;
int RendererImplementation::_height = 600;
double RendererImplementation::_zoom = -10.0;

void RendererImplementation::timer(int millisec)
{
    glutTimerFunc(millisec, RendererImplementation::timer, millisec);
    glutPostRedisplay();
}

void RendererImplementation::reshape(int width, int height)
{
    RendererImplementation::_width = width;
    RendererImplementation::_height = height;
    glViewport(0, 0, (GLsizei)RendererImplementation::_width, (GLsizei)RendererImplementation::_height);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(RendererImplementation::_width)/RendererImplementation::_height, 0.1f, 100.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RendererImplementation::renderingRoutine()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform camera
    glTranslatef(0, 0, RendererImplementation::_zoom);
    glRotatef(0, 1, 0, 0);   // pitch
    glRotatef(0, 0, 1, 0);   // heading

    if (RendererImplementation::_vertex_buffers.empty() || RendererImplementation::_inbex_buffer.empty())
        return;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> normals;
    auto& vboId = RendererImplementation::_vertex_buffers.front();
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    auto total = std::accumulate(RendererImplementation::_rendering_buffer.begin(), RendererImplementation::_rendering_buffer.end(), 0, [](auto first, const auto &second) {
        return  first + second.vertices_size() * 3;
    });
    vertices.reserve(total);
    colors.reserve(total);
    normals.reserve(total);
    for (auto &m : RendererImplementation::_rendering_buffer)
    {
        const auto &color = m.color();
        for (const auto &v : m.vertices())
        {
            vertices.push_back(v.x());
            vertices.push_back(v.y());
            vertices.push_back(v.z());

            colors.push_back(color.r());
            colors.push_back(color.g());
            colors.push_back(color.b());
        }

        for (const auto &n : m.normals())
        {
            normals.push_back(n.x());
            normals.push_back(n.y());
            normals.push_back(n.z());
        }
    }

    const auto vertices_size = vertices.size() * sizeof(decltype(vertices)::value_type);
    const auto colors_size = colors.size() * sizeof(decltype(colors)::value_type);
    const auto normals_size = normals.size() * sizeof(decltype(normals)::value_type);

    glBufferData(GL_ARRAY_BUFFER, vertices_size + colors_size + normals_size, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size, normals_size, &normals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertices_size + normals_size, colors_size, &colors[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    auto itotal = std::accumulate(RendererImplementation::_rendering_buffer.begin(), RendererImplementation::_rendering_buffer.end(), 0, [](auto sum, const auto &entry) {
        return sum + entry.facets_size() * 3;
    });
    std::vector<GLuint> indices;
    indices.reserve(itotal);
    auto &iboId = RendererImplementation::_inbex_buffer.front();
    for (auto &m : RendererImplementation::_rendering_buffer)
    {
        for (const auto &f : m.facets())
        {
            indices.push_back(f[0]);
            indices.push_back(f[1]);
            indices.push_back(f[2]);
        }
    }

    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(decltype(indices)::value_type), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (RendererImplementation::_vbo_supported)
    {
        // bind VBOs with IDs and set the buffer offsets of the bound VBOs
        // When buffer object is bound with its ID, all pointers in gl*Pointer()
        // are treated as offset instead of real pointer.
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

        // enable vertex arrays
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        // before draw, specify vertex and index arrays with their offsets
        glNormalPointer(GL_FLOAT, 0, (void*)vertices_size);
        glColorPointer(3, GL_FLOAT, 0, (void*)vertices_size + normals_size);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawElements(GL_TRIANGLES,            // primitive type
                       indices.size(),//RendererImplementation::_indices.size(),                      // # of indices
                       GL_UNSIGNED_INT,         // data type
                       (void*)0);               // ptr to indices
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        // it is good idea to release VBOs with ID 0 after use.
        // Once bound with 0, all pointers in gl*Pointer() behave as real
        // pointer, so, normal vertex array operations are re-activated
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // draw a cube using vertex array method
    // notice that only difference between VBO and VA is binding buffers and offsets
    else
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        // before draw, specify vertex arrays
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glColorPointer(3, GL_FLOAT, 0, &colors[0]);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

        glDrawElements(GL_TRIANGLES,            // primitive type
                       indices.size(),                      // # of indices
                       GL_UNSIGNED_INT,         // data type
                       (void*)&indices[0]);         // ptr to indices

        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    glPopMatrix();

    glutSwapBuffers();
}

RendererImplementation::RendererImplementation(RendererConfig &&config)
:
_config(config)
{
    RendererImplementation::_vertex_buffers.push_back(0);
    RendererImplementation::_inbex_buffer.push_back(0);
    RendererImplementation::_zoom = config.camera_zoom;
    RendererImplementation::_width = config.initial_width;
    RendererImplementation::_height = config.initial_height;
}

RendererImplementation::~RendererImplementation()
{
    _rendering_buffer.clear();
    for (auto &v : RendererImplementation::_vertex_buffers)
    {
        glDeleteBuffers(1, &v);
    }

    for (auto &i : RendererImplementation::_inbex_buffer)
    {
        glDeleteBuffers(1, &i);
    }

    RendererImplementation::_inbex_buffer.clear();
    RendererImplementation::_vertex_buffers.clear();
}


void RendererImplementation::initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(_config.clear_color.r(), _config.clear_color.g(), _config.clear_color.b(), _config.clear_color.a());                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

void RendererImplementation::initGLUT()
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    int i = 1;
    char * argv[1];
    argv[0] = const_cast<char*>( _config.window_caption.c_str());
    glutInit(&i, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(_config.initial_width, _config.initial_height);  // window size

    glutInitWindowPosition(_config.initial_position.x(), _config.initial_position.y());               // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    _window_handle = glutCreateWindow(argv[0]);         // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(renderingRoutine);
    glutTimerFunc(33, timer, 33);                 // redraw only every given millisec
    glutReshapeFunc(reshape);

    //TODO: leave it for future use
    /*glutKeyboardFunc([] (uint8_t, int, int) {});
    glutMouseFunc([](int, int, int, int) {});
    glutMotionFunc([](int, int){});*/
}

void RendererImplementation::initGLEW()
{
    auto status = glewInit();
    if (status != GLEW_OK)
    {
        throw std::runtime_error("Cannot init GLEW");
    }
    glExtension& ext = glExtension::getInstance();
    RendererImplementation::_vbo_supported = ext.isSupported("GL_ARB_vertex_buffer_object");
}

void RendererImplementation::startGLUTMainLoop()
{
    glutMainLoop();
}

void RendererImplementation::initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

const RendererConfig &RendererImplementation::config() const
{
    return _config;
}

void RendererImplementation::addObjectToBuffer(geometry::Mesh &&object)
{
    _rendering_buffer.push_back(object);
}

void RendererImplementation::clearObjectBuffer()
{
    _rendering_buffer.clear();
}

std::unique_ptr<RendererImplementation> Renderer::_renderer_instance = nullptr;
std::atomic<bool> Renderer::_initialized = false;
std::atomic<size_t> Renderer::_instance_counter = 0;
std::mutex Renderer::_op_mutex;

Renderer::Renderer(RendererConfig &&config)
{
    {
        std::lock_guard lg { _op_mutex };
        if (!_renderer_instance)
        {
            _renderer_instance = std::make_unique<RendererImplementation>(std::move(config));
            if (!_renderer_instance)
                throw std::runtime_error("Can't create renderer implementation");
            try
            {
                _renderer_instance->initGLUT();
                _renderer_instance->initGL();
                _renderer_instance->initGLEW(); //required for VBO
            }
            catch(const std::exception& e)
            {
                //TODO: add logging
                auto ptr = _renderer_instance.release();
                delete ptr;
                throw;
            }
        }
    }

    (void)_instance_counter.fetch_add(1);
}

Renderer::~Renderer()
{
    if (Renderer::_instance_counter.fetch_sub(1))
    {
        {
            std::lock_guard lg { _op_mutex };
            auto ptr = _renderer_instance.release();
            delete ptr;
            _renderer_instance = nullptr;
        }
    }    
}

const RendererConfig &Renderer::config() const
{
    return _renderer_instance->config();
}

void Renderer::launch()
{
    this->_renderer_instance->startGLUTMainLoop();
}

void Renderer::addObject(rcbe::geometry::Mesh &&mesh)
{
    _renderer_instance->addObjectToBuffer(std::move(mesh));
}

/*std::future<typename std::result_of<std::function<void(void)>>::type> Renderer::launch()
{
    const auto invoker = [this]() -> void {
        this->_renderer_instance->startGLUTMainLoop();
    };
    auto renderer_future = std::async(std::launch::async, invoker);
}*/
}