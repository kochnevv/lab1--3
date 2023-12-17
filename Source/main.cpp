#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include <vector>
#include "SDL.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"



void HandleUserInput(sf::RenderWindow& window, const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;
	default:
		break;
	}
}

void Update(sf::RenderWindow& window, const sf::Time& deltaClock)
{
	// Make some time-dependent updates, like: physics, gameplay logic, animations, etc.
}
sf::Sprite sprite;
sf::Texture spriteTexture;
sf::Image spriteImage;

void Render(sf::RenderWindow& window)
{
	// Draw some sfml/opengl items
	static sf::CircleShape circle = []()
	{
		sf::CircleShape circle;
		circle.setRadius(50);
		circle.setPosition(200, 200);
		circle.setFillColor(sf::Color::White);
		return circle;
	}();
	window.draw(circle);
	window.draw(sprite);
}

void RenderGui(sf::RenderWindow& window)
{
	ImGui::Begin("Default window");
	ImGui::End();
}


int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL init falure\n";
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	SDL_Window *window =
		SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::cout << "Couldn't create window\n";
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cout << "Couldn't create renderer\n";
		return -1;
	}

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	bool programWorks = true;
	int fps = 60;
	Uint64 trueFrameTime = 1000.f / fps;

	int textureWidth = SDL_GetWindowSurface(window)->w;
	int textureHeight = SDL_GetWindowSurface(window)->h;

	SDL_Color Color[500];

	for (int i = 0; i < textureHeight; i++)
	{
		if ((i + int(float(textureWidth / 256))) > 255)
			Color[i].r = 255;
		else
			Color[i].r = i;
		if ((i + int(float(textureWidth / 256))) > 255)
			Color[i].b = 255;
		else
			Color[i].b = i;
		Color[i].g = 0;
		Color[i].a = 0;
	}


	while (programWorks)
	{
		Uint64 frameTime = SDL_GetTicks();
		// HandleInputs
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				ImGui_ImplSDL2_ProcessEvent(&e);
				if (e.type == SDL_QUIT)
					programWorks = false;
			}
		}

		// UpdateImage
		{
			ImGui_ImplSDLRenderer_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
			// Render Gui
			{
				ImGui::Begin("Window");
				if (ImGui::Button("Lineynoe"))
				{
					for (int i = 0; i < textureHeight; i++)
					{
						SDL_SetRenderDrawColor(renderer, Color[i].r, Color[i].g, Color[i].b, Color[i].a);
						SDL_RenderDrawLine(renderer, 0, i, textureWidth, i);
					}
				}
				if (ImGui::Button("Krugovoe"))
				{
					for (int i = textureHeight; i > 0; i--)
					{
						SDL_SetRenderDrawColor(renderer, Color[i].r, Color[i].g, Color[i].b, Color[i].a);
						int newX = 0;
						int newY = i;
						int circleDiametr = i - 1;
						int flag = 0;

						while (newY >= newX)
						{

							flag += SDL_RenderDrawLine(renderer, textureWidth / 2 - newY, textureHeight / 2 + newX,
													   textureWidth / 2 + newY, textureHeight / 2 + newX);
							flag += SDL_RenderDrawLine(renderer, textureWidth / 2 - newX, textureHeight / 2 + newY,
													   textureWidth / 2 + newX, textureHeight / 2 + newY);
							flag += SDL_RenderDrawLine(renderer, textureWidth / 2 - newX, textureHeight / 2 - newY,
													   textureWidth / 2 + newX, textureHeight / 2 - newY);
							flag += SDL_RenderDrawLine(renderer, textureWidth / 2 - newY, textureHeight / 2 - newX,
													   textureWidth / 2 + newY, textureHeight / 2 - newX);

							if (flag < 0)
							{
								flag = -1;
								break;
							}

							if (circleDiametr >= 2 * newX)
							{
								circleDiametr -= 2 * newX + 1;
								newX += 1;
							}
							else if (circleDiametr < 2 * (i - newY))
							{
								circleDiametr += 2 * newY - 1;
								newY -= 1;
							}
							else
							{
								circleDiametr += 2 * (newY - newX - 1);
								newY -= 1;
								newX += 1;
							}
						}
					}
				}


				ImGui::End();
			}

			ImGui::Render();

			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
			SDL_RenderPresent(renderer);
		}

		frameTime = SDL_GetTicks() - frameTime;
		if (frameTime < trueFrameTime)
			SDL_Delay(trueFrameTime - frameTime);
	}

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
