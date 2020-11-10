#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#define LARGURA 640
#define ALTURA 480
#define FPS 60.0
#define FrameFps 20.0
#define Chao 411



struct objeto
{
	ALLEGRO_BITMAP* imagem;
	float x;
	float y;
	int largura;
	int altura;
};

typedef struct objeto Objeto;

Objeto* personagem, * sprite_parado, * goblin, * item;


ALLEGRO_BITMAP* frame = NULL;
ALLEGRO_BITMAP* frame2 = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_TIMER* frametimer = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_BITMAP* background = NULL;

void inicialização() {

	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();

	timer = al_create_timer(1.0 / FPS);
	frametimer = al_create_timer(1.0 / FrameFps);

	janela = al_create_display(LARGURA, ALTURA);

	fila_eventos = al_create_event_queue();

	al_set_window_title(janela, "Em busca do Hardware Perdido");

	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_eventos, al_get_timer_event_source(frametimer));

}

int main(void) {

	inicialização();

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_flip_display();
	

	// Carrega imagem
	background = al_load_bitmap("imagens/menu1.jpg");


	personagem = (Objeto*)malloc(sizeof(Objeto));
	personagem->imagem = al_load_bitmap("Sprites/MC_Sprite_walk.png");
	personagem->largura = 490;
	personagem->altura = 49;
	personagem->y = Chao - personagem->altura;
	personagem->x = 0;

	sprite_parado = (Objeto*)malloc(sizeof(Objeto));
	sprite_parado->imagem = al_load_bitmap("Sprites/MC_Sprite_idle.png");
	sprite_parado->x = personagem->x;
	sprite_parado->largura = 440;
	sprite_parado->altura = 49;
	sprite_parado->y = personagem->y;

	/*goblin = (Objeto*)malloc(sizeof(Objeto));
	goblin->imagem = al_load_bitmap("Sprites/goblins.png");
	goblin->x = 250;
	goblin->y = Chao - goblin->altura;
	goblin->largura = 256;
	goblin->altura = 49;*/


	// Variaveis de controle de menu
	int menu = 1, jogar = 0, creditos = 0, infos = 0;

	int pressionadox = 0;
	int i = 0;
	int aux = 0;
	int j = 0;
	int k = 0;

	bool draw = false, draw2 = true, ativo = false;
	int pressionando = 0;
	float velocidade_movimento = 4.5;
	float velx, vely;
	velx = 0;
	vely = 0;

	bool pulando = false;
	float velocidade_pulo = 15;

	const float gravidade = 0.80;

	int sourceX = 0, sourceX_inimigo = 0;

	bool inimigo1 = true, inimigo2 = true;
	int velocidade_inimigo = 1.5;
	
	ALLEGRO_KEYBOARD_STATE key_state;


	al_start_timer(timer);
	al_start_timer(frametimer);
	//Looping principal
	while (true) {

		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);

		al_get_keyboard_state(&key_state);

		// Funcao do menu do jogo
		// Ja começa com 1 pois ela irá controlar os demais whiles, as outras opções
		while (menu == 1) {

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_flip_display();
			
			// Se o evento for um clique do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				// Se o clique foi no botão sair
				if ((evento.mouse.x >= 590 &&
					evento.mouse.x <= 630 && evento.mouse.y <= 42 &&
					evento.mouse.y >= 6)) {

					// Fecha o jogo
					menu = 0;
					jogar = 1;
				}
			}

		}

		while (jogar == 1) {

			if (evento.type == ALLEGRO_EVENT_TIMER) {

				if (evento.timer.source == timer) {

					ativo = true;

					pressionando = 0;

					if (al_key_down(&key_state, ALLEGRO_KEY_UP) && pulando) {
						vely = -velocidade_pulo;
						pulando = false;
					}
					if (al_key_down(&key_state, ALLEGRO_KEY_RIGHT)) {
						velx = velocidade_movimento;

						j = 1;

						pressionando = 1;
					}
					else if (al_key_down(&key_state, ALLEGRO_KEY_LEFT)) {
						velx = -velocidade_movimento;

						j = 0;

						pressionando = 1;
					}
					else {
						ativo = false;
						velx = 0;
					}

					if (!pulando) {
						vely += gravidade;
					}
					else {
						vely = 0;
					}
					personagem->x += velx;
					personagem->y += vely;

					pulando = (personagem->y + 49 >= 480);

					if (pulando) {
						personagem->y = 480 - 49;
					}

				}


				if (evento.timer.source == frametimer) {
					if (ativo) {
						sourceX += al_get_bitmap_width(personagem->imagem) / 10;
					}
					else {
						sourceX = 0;
					}
					if (sourceX >= al_get_bitmap_width(personagem->imagem)) {
						sourceX = 0;
					}

					if (pressionando == 1) {
						draw = true;
					}

					if (pressionando == 0) {

						i++;

						if (i > 10) {
							i = 0;
						}

						draw2 = true;
					}
				}

			}
			if (draw) {
				draw = false;

				//frame = al_create_sub_bitmap(personagem->imagem, (personagem->largura / 10) * i, 0, personagem->largura / 10 - 5, personagem->altura);

				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap_region(personagem->imagem, sourceX, 0, personagem->largura / 10, personagem->altura, personagem->x, personagem->y, j);
				al_flip_display();

			}
			else if (draw2) {
				draw2 = false;

				frame2 = al_create_sub_bitmap(sprite_parado->imagem, (sprite_parado->largura / 11) * i, 0, sprite_parado->largura / 11, sprite_parado->altura);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(frame2, personagem->x, personagem->y, j);
				al_flip_display();
			}

		}
		while (creditos == 1) {

		}
		while (infos == 1) {

		}

	}

	al_destroy_display(janela);
	al_destroy_bitmap(background);

	return 0;
}