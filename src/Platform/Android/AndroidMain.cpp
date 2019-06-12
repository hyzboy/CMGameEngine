/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

/**
* Our saved state data.
*/
struct saved_state
{
    float angle;
    int32_t x;
    int32_t y;
};

/**
* Shared state for our app.
*/
struct engine
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;
    ASensorEvent acceleration_event;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(struct engine* engine)
{
    // initialize OpenGL ES and EGL

    /*
    * Here specify the attributes of the desired configuration.
    * Below, we select an EGLConfig with at least 8 bits per color
    * component compatible with on-screen windows
    */
    const EGLint config_attrs[] =
    {
        EGL_DEPTH_SIZE,         0,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_STENCIL_SIZE,       8,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,         //即使是用es3这里也写es2
        EGL_NONE
    };

    const EGLint context_attrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION,    3,
        EGL_NONE
    };

    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    EGLint config_count;
    bool depth_check=false;
    const EGLint depth_bit_sizes[]={24,16};

    for(uint i=0;i<sizeof(depth_bit_sizes)/sizeof(EGLint);i++)
    {
        config_attrs[1]=depth_bit_sizes[i];

        if(eglChooseConfig(display,config_attrs,&egl_config,1,&config_count)==EGL_TRUE)
            if(config_count>0)
            {
                depth_check=true;
                break;
            }
    }

    if(!depth_check)
        return(-1);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
    * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
    * As soon as we picked a EGLConfig, we can safely reconfigure the
    * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, nullptr);

    context = eglCreateContext(display, config, nullptr, context_attrs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Initialize GL state.
    Cube_setupGL(w, h);

    return 0;
}

/**
* Just the current frame in the display.
*/

static void engine_draw_frame(struct engine* engine)
{
    if (engine->display == nullptr) {
        // No display.
        return;
    }

    Cube_prepare();

    Cube_draw();

    eglSwapBuffers(engine->display, engine->surface);
}


/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(struct engine* engine)
{
    if (engine->display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (engine->context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(engine->display, engine->context);
        }

        if (engine->surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(engine->display, engine->surface);
        }

        eglTerminate(engine->display);
    }

    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;

    Cube_tearDownGL();
}

/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    struct engine* engine = (struct engine*)app->userData;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }

    return 0;
}

/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;

    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:    // The system has asked us to save our current state.  Do so.
                                    engine->app->savedState = malloc(sizeof(struct saved_state));
                                    *((struct saved_state*)engine->app->savedState) = engine->state;
                                    engine->app->savedStateSize = sizeof(struct saved_state);
                                    break;

        case APP_CMD_INIT_WINDOW:   // The window is being shown, get it ready.
                                    if (engine->app->window != nullptr)
                                    {
                                        engine_init_display(engine);
                                        engine_draw_frame(engine);
                                    }
                                    break;

        case APP_CMD_TERM_WINDOW:   // The window is being hidden or closed, clean it up.
                                    engine_term_display(engine);
                                    break;

        case APP_CMD_GAINED_FOCUS:  // When our app gains focus, we start monitoring the accelerometer.
                                    if (engine->accelerometerSensor != nullptr)
                                    {
                                        ASensorEventQueue_enableSensor(engine->sensorEventQueue,engine->accelerometerSensor);
                                        // We'd like to get 60 events per second (in us).
                                        ASensorEventQueue_setEventRate(engine->sensorEventQueue,engine->accelerometerSensor, (1000L / 60) * 1000);
                                    }
                                    break;

        case APP_CMD_LOST_FOCUS:    // When our app loses focus, we stop monitoring the accelerometer.
                                    // This is to avoid consuming battery while not being used.
                                    if (engine->accelerometerSensor != nullptr)
                                    {
                                        ASensorEventQueue_disableSensor(engine->sensorEventQueue,engine->accelerometerSensor);
                                    }
                                    // Also stop animating.
                                    engine->animating = 0;
                                    engine_draw_frame(engine);
                                    break;
    }
}

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state)
{
    struct engine engine;

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager       = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue    = ASensorManager_createEventQueue(engine.sensorManager,state->looper, LOOPER_ID_USER, nullptr, nullptr);

    if (state->savedState != nullptr)
    {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    engine.animating = 1;

    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, nullptr, &events,(void**)&source)) >= 0)
        {
            // Process this event.
            if (source != nullptr)
            {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER)
            {
                if (engine.accelerometerSensor != nullptr)
                {
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,&engine->acceleration_event;, 1) > 0)
                    {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                            engine->acceleration_event.acceleration.x,
                            engine->acceleration_event.acceleration.y,
                            engine->acceleration_event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0)
            {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating)
        {
            // Done with events; draw next animation frame.
            Cube_update();
            //if (engine.state.angle > 1) {
            //  engine.state.angle = 0;
            //}

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
