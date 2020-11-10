#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#define LARGURA 640
#define ALTURA 480


ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_BITMAP* background = NULL;

int main(void) {

	// Inicializações
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();

	// Cria janela
	janela = al_create_display(LARGURA, ALTURA);

	fila_eventos = al_create_event_queue();
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_flip_display();
	

	// Carrega imagem
	background = al_load_bitmap("imagens/menu1.jpg");

	// Variaveis de controle de menu
	int menu = 1, jogar = 0, creditos = 0, infos = 0;
	
	

	// Coisas de eventos
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));


	//Looping principal
	while (true) {

		//declara vriavel que recebe evento e timeout
		ALLEGRO_EVENT evento;
		//espero por um evento da fila, e guarda em evento
		al_wait_for_event(fila_eventos, &evento);

		al_register_event_source(fila_eventos, al_get_keyboard_event_source());
		al_register_event_source(fila_eventos, al_get_mouse_event_source());
		al_register_event_source(fila_eventos, al_get_display_event_source(janela));

		// Funcao do menudo jogo
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
				}
			}

		}

		while (jogar == 0) {

	

		}
		while (creditos == 0) {

		}
		while (infos == 0) {

		}

	}

	al_destroy_display(janela);
	al_destroy_bitmap(background);

	return 0;
}