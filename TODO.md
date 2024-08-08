# kEngine To-do List

## Graphics
#### Renderer
- [ ] Create RendererContext class/namepsace to manage rendering and prevent multiple other classes needing to store instance reference.

#### Text
- [x] Implement text rendering using [`SDL_ttf`](https://www.libsdl.org/projects/SDL_ttf/) library.

#### UI
- [x] Implement primitive UI classes (Window, Button, Label, etc.)
- [ ] Have UIElement give default constructor, getters and setters?
- [ ] Create UIManager to handle UI updating, rendering

## Core Components
- [x] Use distance scaling through world->screen conversions to allow for use of real physical values?
- [x] Refactor Body class to src

#### Camera
- [x] Implement Camera system (allow for movement around scene and zoom - potential savings if culling used)
- [x] Refactor zoom to use linear interpolation towards a target zoom
- [x] Clamp target zoom and don't allow zoom input if request is beyond clamp
- [ ] ***BUG*** When the zoom is close enough to the target zoom, the LERP stops and the zoom is set to exactly the target zoom. This is can cause a slight jitter into place.  
