# Oasis Engine

## Introduction
Oasis engine is a game engine designed with ECS architecture in mind, with the purpose of providing a useful and elegant framework for quick prototyping. The engine avoids usage of a rendering library, and as such the project utilises a software rendering approach.

### Motivation
Oasis engine is a game engine motivated by my desire to achieve further understanding of the paradigms of project architecture, data driven programming, the rendering pipeline, and hardware software.

## Features
- Modular Rendering Engine
- Entity Component System (ECS) for performant entity management.
- Basic geometric rendering capabilities.
- Camera and scene management.

## Next up: 
- Z buffering.
- Fix input 'sticking' upon changing window focus.
- Texturing.
- Indices.
- Collision.
- Physics.
- ECS: define systems after entities, inform systems.
- Events, triggers, etc.
- Fix w2s such that NDC coordinates aren't set to 0 if off screen, ruining entire triangle.
- Multithreaded rendering.

Smaller things
- lock mouse mechanism, unlock, mouse pos, delta

- remove camera.h, refactor renderer and renderer3d, refactor interaction between managers and engine
- clean up math functions
- clean up example_game

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)

## Installation
To set up Oasis Engine on your local machine:
1. Ensure you have CMake and a C++ compiler (Clang++ recommended)
2. Clone the repository `git clone https://github.com/benfrag/oasis-engine.git`
3. Navigate to the project directory and run `cmake .` to generate build files.
4. Build the project using your build system.

## Usage
- The repository includes an example usage with the 'example_game' folder outlining the basic principles of utilising the engine.
- **Configuration**: create an instance of EngineCore, and call its initialize method, optionally you can configure the window it creates by calling the configure_window method.
- **Creating Entities**: Use the ECSController in the EngineCore to create and manage entities.
- **Rendering**: Use the base_components and base_systems to render geometry.

## Example Usage
Below is a basic example of how to initialise and run the engine in a C++ application:

```
#include <windows.h>
#include <iostream>

#include "engine/core/engine.h"

int main()
{
    EngineCore engine;

    try
    {
        WindowConfig window_config;
        window_config.width = 800;
        window_config.height = 600;
        engine.configure_window(window_config);
        engine.initialize();

// Utilise the engine to create entities, provide data for these entities through creating components, and register systems to act on this data.

        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exception occured: " << e.what() << std::endl;
    }

    engine.shutdown();

    return 0;
}
```

## Example ECS Usage:
to be added.

## License
Oasis Engine is released under the [MIT License](LICENSE.md).
