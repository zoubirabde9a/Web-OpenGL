call del main.wasm
call del main.html
call del main.js
call em++ -std=c++14 -O2 -s WASM=0 -s USE_FREETYPE=1 -s USE_WEBGL2=1 -s USE_SDL=2 -s FULL_ES3=1 -DEMSCRIPTEN -I include json_reader.cpp json_value.cpp json_writer.cpp freetype-gl/distance-field.cpp freetype-gl/edtaa3func.cpp freetype-gl/platform.cpp freetype-gl/texture-atlas.cpp freetype-gl/texture-font.cpp freetype-gl/utf8-utils.cpp freetype-gl/vector.cpp button.cpp Slider.cpp ZWindow.cpp ImageButton.cpp PushButton.cpp jsonfunctions.cpp utils.cpp vertex.cpp spritefont.cpp glslprogram.cpp spritebatch.cpp imageloader.cpp debugrenderer.cpp batchrenderer.cpp picopng.cpp tilesheet.cpp resourcemanager.cpp fontmanager.cpp zgui.cpp camera2d.cpp inputmanager.cpp iomanager.cpp window.cpp main.cpp -o main.html --embed-file Shaders\textureShading_es.frag --memory-init-file 0 --embed-file Shaders\textureShading_es.vert --embed-file Resources\a.png --embed-file Font\arial.ttf --embed-file Font\chintzy.ttf --embed-file Font\comic.ttf --embed-file Resources\AlfiskoSkin.png --embed-file Resources\sDropDownListHover.png --embed-file Resources\sDropDownListNormal.png --embed-file Resources\sDropDownListPushed.png --embed-file Resources\sWindow.png --embed-file Scripts\scheme.json --embed-file Scripts\imageSets.json --embed-file Resources\Cross.png --embed-file Resources\Rock.png && main.html