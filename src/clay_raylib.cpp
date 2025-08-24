#include <clay_raylib.hpp>

Ray GetScreenToWorldPointWithZDistance(
    Vector2 position, Camera camera, int screenWidth, int screenHeight, float zDistance
) {
    Ray ray = { 0 };

    // Calculate normalized device coordinates
    // NOTE: y value is negative
    float x = ((2.0F * position.x) / (float)screenWidth) - 1.0F;
    float y = 1.0F - ((2.0F * position.y) / (float)screenHeight);
    float z = 1.0F;

    // Store values in a vector
    Vector3 deviceCoords = { x, y, z };

    // Calculate view matrix from camera look at
    Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);

    Matrix matProj = MatrixIdentity();

    if (camera.projection == CAMERA_PERSPECTIVE) {
        // Calculate projection matrix from perspective
        matProj = MatrixPerspective(
            camera.fovy * DEG2RAD, ((double)screenWidth / (double)screenHeight), 0.01F, zDistance
        );
    } else if (camera.projection == CAMERA_ORTHOGRAPHIC) {
        double aspect = (double)screenWidth / (double)screenHeight;
        double top = camera.fovy / 2.0;
        double right = top * aspect;

        // Calculate projection matrix from orthographic
        matProj = MatrixOrtho(-right, right, -top, top, 0.01, 1000.0);
    }

    // Unproject far/near points
    Vector3 nearPoint =
        Vector3Unproject((Vector3){ deviceCoords.x, deviceCoords.y, 0.0F }, matProj, matView);
    Vector3 farPoint =
        Vector3Unproject((Vector3){ deviceCoords.x, deviceCoords.y, 1.0F }, matProj, matView);

    // Calculate normalized direction vector
    Vector3 direction = Vector3Normalize(Vector3Subtract(farPoint, nearPoint));

    ray.position = farPoint;

    // Apply calculated vectors to ray
    ray.direction = direction;

    return ray;
}

Clay_Dimensions
Raylib_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData) {
    // Measure string size for Font
    Clay_Dimensions textSize = { 0 };

    float maxTextWidth = 0.0F;
    float lineTextWidth = 0;
    int maxLineCharCount = 0;
    int lineCharCount = 0;

    float textHeight = config->fontSize;
    Font* fonts = (Font*)userData;
    Font fontToUse = fonts[config->fontId];
    // Font failed to load, likely the fonts are in the wrong place relative to the execution dir.
    // RayLib ships with a default font, so we can continue with that built in one.
    if (fontToUse.glyphs == nullptr) {
        fontToUse = GetFontDefault();
    }

    float scaleFactor = (float)config->fontSize / (float)fontToUse.baseSize;

    for (int i = 0; i < text.length; ++i, lineCharCount++) {
        if (text.chars[i] == '\n') {
            maxTextWidth = fmax(maxTextWidth, lineTextWidth);
            maxLineCharCount = CLAY__MAX(maxLineCharCount, lineCharCount);
            lineTextWidth = 0;
            lineCharCount = 0;
            continue;
        }
        int index = text.chars[i] - 32;
        if (fontToUse.glyphs[index].advanceX != 0) {
            lineTextWidth += (float)fontToUse.glyphs[index].advanceX;
        } else {
            lineTextWidth +=
                (fontToUse.recs[index].width + (float)fontToUse.glyphs[index].offsetX);
        }
    }

    maxTextWidth = fmax(maxTextWidth, lineTextWidth);
    maxLineCharCount = CLAY__MAX(maxLineCharCount, lineCharCount);

    textSize.width =
        maxTextWidth * scaleFactor + (float)(lineCharCount * config->letterSpacing);
    textSize.height = textHeight;

    return textSize;
}

void Clay_Raylib_Initialize(int width, int height, const char* title, unsigned int flags) {
    SetConfigFlags(flags);
    InitWindow(width, height, title);
    //    EnableEventWaiting();
}

void Clay_Raylib_Close() {
    if (temp_render_buffer != nullptr) {
        free(temp_render_buffer);
    }
    temp_render_buffer_len = 0;

    CloseWindow();
}

void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands, Font* fonts) {
    for (int j = 0; j < renderCommands.length; j++) {
        Clay_RenderCommand* renderCommand =
            Clay_RenderCommandArray_Get(&renderCommands, j);
        Clay_BoundingBox boundingBox = { roundf(renderCommand->boundingBox.x),
                                         roundf(renderCommand->boundingBox.y),
                                         roundf(renderCommand->boundingBox.width),
                                         roundf(renderCommand->boundingBox.height) };
        switch (renderCommand->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            Clay_TextRenderData* textData = &renderCommand->renderData.text;
            Font fontToUse = fonts[textData->fontId];

            int strlen = textData->stringContents.length + 1;

            if (strlen > temp_render_buffer_len) {
                // Grow the temp buffer if we need a larger string
                if (temp_render_buffer != nullptr) {
                    free(temp_render_buffer);
                }
                temp_render_buffer = (char*)malloc(strlen);
                temp_render_buffer_len = strlen;
            }

            // Raylib uses standard C strings so isn't compatible with cheap slices, we need to clone the string to append null terminator
            memcpy(
                temp_render_buffer, textData->stringContents.chars,
                textData->stringContents.length
            );
            temp_render_buffer[textData->stringContents.length] = '\0';
            DrawTextEx(
                fontToUse, temp_render_buffer,
                (Vector2){ boundingBox.x, boundingBox.y },
                (float)textData->fontSize, (float)textData->letterSpacing,
                CLAY_COLOR_TO_RAYLIB_COLOR(textData->textColor)
            );

            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Texture2D imageTexture =
                *(Texture2D*)renderCommand->renderData.image.imageData;
            Clay_Color tintColor = renderCommand->renderData.image.backgroundColor;
            if (tintColor.r == 0 && tintColor.g == 0 && tintColor.b == 0
                && tintColor.a == 0) {
                tintColor = (Clay_Color){ 255, 255, 255, 255 };
            }
            DrawTexturePro(
                imageTexture,
                (Rectangle){ 0, 0, (float)imageTexture.width,
                             (float)imageTexture.height },
                (Rectangle){ boundingBox.x, boundingBox.y, boundingBox.width,
                             boundingBox.height },
                (Vector2){}, 0, CLAY_COLOR_TO_RAYLIB_COLOR(tintColor)
            );
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
            BeginScissorMode(
                (int)roundf(boundingBox.x), (int)roundf(boundingBox.y),
                (int)roundf(boundingBox.width), (int)roundf(boundingBox.height)
            );
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
            EndScissorMode();
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            Clay_RectangleRenderData* config = &renderCommand->renderData.rectangle;
            if (config->cornerRadius.topLeft > 0) {
                float radius = (config->cornerRadius.topLeft * 2)
                    / ((boundingBox.width > boundingBox.height) ? boundingBox.height
                                                                : boundingBox.width);
                DrawRectangleRounded(
                    (Rectangle){ boundingBox.x, boundingBox.y,
                                 boundingBox.width, boundingBox.height },
                    radius, 8, CLAY_COLOR_TO_RAYLIB_COLOR(config->backgroundColor)
                );
            } else {
                DrawRectangle(
                    (int)boundingBox.x, (int)boundingBox.y, (int)boundingBox.width,
                    (int)boundingBox.height, CLAY_COLOR_TO_RAYLIB_COLOR(config->backgroundColor)
                );
            }
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            Clay_BorderRenderData* config = &renderCommand->renderData.border;
            // Left border
            if (config->width.left > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x),
                    (int)roundf(boundingBox.y + config->cornerRadius.topLeft),
                    (int)config->width.left,
                    (int)roundf(
                        boundingBox.height - config->cornerRadius.topLeft
                        - config->cornerRadius.bottomLeft
                    ),
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            // Right border
            if (config->width.right > 0) {
                DrawRectangle(
                    (int)roundf(
                        boundingBox.x + boundingBox.width - (float)config->width.right
                    ),
                    (int)roundf(boundingBox.y + config->cornerRadius.topRight),
                    (int)config->width.right,
                    (int)roundf(
                        boundingBox.height - config->cornerRadius.topRight
                        - config->cornerRadius.bottomRight
                    ),
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            // Top border
            if (config->width.top > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x + config->cornerRadius.topLeft),
                    (int)roundf(boundingBox.y),
                    (int)roundf(
                        boundingBox.width - config->cornerRadius.topLeft
                        - config->cornerRadius.topRight
                    ),
                    (int)config->width.top, CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            // Bottom border
            if (config->width.bottom > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x + config->cornerRadius.bottomLeft),
                    (int)roundf(
                        boundingBox.y + boundingBox.height
                        - (float)config->width.bottom
                    ),
                    (int)roundf(
                        boundingBox.width - config->cornerRadius.bottomLeft
                        - config->cornerRadius.bottomRight
                    ),
                    (int)config->width.bottom, CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            if (config->cornerRadius.topLeft > 0) {
                DrawRing(
                    (Vector2){ roundf(boundingBox.x + config->cornerRadius.topLeft),
                               roundf(boundingBox.y + config->cornerRadius.topLeft) },
                    roundf(config->cornerRadius.topLeft - (float)config->width.top),
                    config->cornerRadius.topLeft, 180, 270, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            if (config->cornerRadius.topRight > 0) {
                DrawRing(
                    (Vector2){ roundf(
                                   boundingBox.x + boundingBox.width
                                   - config->cornerRadius.topRight
                               ),
                               roundf(boundingBox.y + config->cornerRadius.topRight) },
                    roundf(config->cornerRadius.topRight - (float)config->width.top),
                    config->cornerRadius.topRight, 270, 360, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            if (config->cornerRadius.bottomLeft > 0) {
                DrawRing(
                    (Vector2){ roundf(boundingBox.x + config->cornerRadius.bottomLeft),
                               roundf(
                                   boundingBox.y + boundingBox.height
                                   - config->cornerRadius.bottomLeft
                               ) },
                    roundf(
                        config->cornerRadius.bottomLeft - (float)config->width.bottom
                    ),
                    config->cornerRadius.bottomLeft, 90, 180, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            if (config->cornerRadius.bottomRight > 0) {
                DrawRing(
                    (Vector2){ roundf(
                                   boundingBox.x + boundingBox.width
                                   - config->cornerRadius.bottomRight
                               ),
                               roundf(
                                   boundingBox.y + boundingBox.height
                                   - config->cornerRadius.bottomRight
                               ) },
                    roundf(
                        config->cornerRadius.bottomRight - (float)config->width.bottom
                    ),
                    config->cornerRadius.bottomRight, 0.1, 90, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color)
                );
            }
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
            Clay_CustomRenderData* config = &renderCommand->renderData.custom;
            auto* customElement = (CustomLayoutElement*)config->customData;
            if (customElement == nullptr) {
                continue;
            }
            switch (customElement->type) {
            case ::CustomLayoutElementType::CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL: {
                Clay_BoundingBox rootBox = renderCommands.internalArray[0].boundingBox;
                float scaleValue = CLAY__MIN(
                    CLAY__MIN(1, 768 / rootBox.height) * CLAY__MAX(1, rootBox.width / 1024), 1.5F
                );
                Ray positionRay = GetScreenToWorldPointWithZDistance(
                    (Vector2){ renderCommand->boundingBox.x
                                   + (renderCommand->boundingBox.width / 2),
                               renderCommand->boundingBox.y
                                   + (renderCommand->boundingBox.height / 2) + 20 },
                    Raylib_camera, (int)roundf(rootBox.width), 140,
                    roundf(rootBox.height)
                );
                BeginMode3D(Raylib_camera);
                DrawModel(
                    customElement->customData.model.model, positionRay.position,
                    customElement->customData.model.scale * scaleValue, WHITE
                ); // Draw 3d model with texture
                EndMode3D();
                break;
            }
            default:
                break;
            }
            break;
        }
        default: {
            printf("Error: unhandled render command.");
            exit(1);
        }
        }
    }
}
