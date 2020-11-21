#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define LARGURA 640
#define ALTURA 480
#define FPS 60.0
#define FrameFps 18.0
#define InimigoFps 11.0
#define Chao 411


struct objeto
{
	ALLEGRO_BITMAP* imagem;
	float x;
	float y;
	int largura;
	int altura;
	int vida;
};

//                       
typedef struct objeto Objeto;

Objeto* personagem, * sprite_parado, * goblin, * espada, * processador, * processador_mini, * placa_mae, * placa_mae_mini, * sprite_atacando, * inventario;


ALLEGRO_FONT* fonte = NULL;
ALLEGRO_BITMAP* frame = NULL;
ALLEGRO_BITMAP* frame2 = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* background_jogo1 = NULL;
ALLEGRO_BITMAP* infoss = NULL;
ALLEGRO_BITMAP* creditoss = NULL;
ALLEGRO_SAMPLE* musica = NULL;
ALLEGRO_SAMPLE_INSTANCE* songInstance = NULL;

//teste
ALLEGRO_BITMAP* placa_mae_inv = NULL;
ALLEGRO_BITMAP* processador_inv = NULL;
ALLEGRO_BITMAP* inventario_processador = NULL;
ALLEGRO_BITMAP* inventario_placa_mae = NULL;


int pressionadox = 0;
int i = 0;
int aux = 0;
int j = 0;
int k = 0;
int l = 0;

bool draw = false, draw2 = true, ativo = false, item_processador = false, item_processador_mini = false, item_placa = false, item_placa_mini = false;

//teste
bool inv_placa = false;
bool inv_processador = false;
bool inventarioo = false;

int pressionando = 0;
float velocidade_movimento = 4.5;
float velx, vely;
velx = 0;
vely = 0;

bool pulando = false, atacando = false, espada_ativa = false;
float velocidade_pulo = 15;

const float gravidade = 0.80;

int sourceX = 0, sourceX_inimigo = 0, sourceX_atacando;

bool inimigo1 = true, inimigo2 = true;
float velocidade_inimigo = 1.5;

float velocidade_projetil = 10.0;
int posicao_projetil = 0;

ALLEGRO_KEYBOARD_STATE key_state;

// Funcao que inicializa as coisas
void inicialização() {

	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();

	//audio
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(5);

	timer = al_create_timer(1.0 / FPS);

	janela = al_create_display(LARGURA, ALTURA);

	fila_eventos = al_create_event_queue();

	al_set_window_title(janela, "Em busca do Hardware Perdido");

	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

}

// Função movimentação
void movimentacao(ALLEGRO_EVENT evento) {


	if (evento.timer.source == timer) {

		ativo = true;

		pressionando = 0;

		if (al_key_down(&key_state, ALLEGRO_KEY_UP) && pulando) {
			vely = -velocidade_pulo;
			pulando = false;
		}
		if (al_key_down(&key_state, ALLEGRO_KEY_Z) && !atacando && !espada_ativa) {
			atacando = true;

			if (!espada_ativa) {
				espada->y = personagem->y;
				l = j;
			}

			espada_ativa = true;
		}
		else if (al_key_down(&key_state, ALLEGRO_KEY_E)) {
			inventarioo = true;
		}
		else if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
			inventarioo = false;
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
		if (personagem->x >= -1 && personagem->x + personagem->largura / 10 <= 639) {
			personagem->x += velx;
		}
		if (personagem->x <= -1) {
			personagem->x = 0;
		}
		else if (personagem->x + personagem->largura / 10 >= 639) {
			personagem->x = 638 - personagem->largura / 10;
		}
		personagem->y += vely;

		pulando = (personagem->y + 49 >= Chao);

		if (pulando) {
			personagem->y = Chao - 49;
		}

		if (inimigo1 && personagem->x - goblin->x < 0) {
			goblin->x -= velocidade_inimigo;
			k = 0;
		}
		else if (inimigo1 && personagem->x - goblin->x > 0) {
			goblin->x += velocidade_inimigo;
			k = 1;
		}

		if (espada_ativa && l == 1) {
			espada->x += velocidade_projetil;
		}
		else if (espada_ativa && l == 0) {
			espada->x -= velocidade_projetil;
		}
		else {
			espada->x = personagem->x;
		}
		
	}


	//if (evento.timer.source == frametimer) {

	if (ativo) {
		sourceX += al_get_bitmap_width(personagem->imagem) / 10;
	}
	else {
		sourceX = 1;
	}
	if (sourceX >= al_get_bitmap_width(personagem->imagem)) {
		sourceX = 1;
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

	if (atacando) {
		sourceX_atacando += al_get_bitmap_width(sprite_atacando->imagem) / 10;
	}
	else {
		sourceX_atacando = 0;
	}
	if (sourceX_atacando >= al_get_bitmap_width(sprite_atacando->imagem)) {
		sourceX_atacando = 0;
		atacando = false;
	}
	//}

		//if (evento.timer.source == inimigotimer) {

	if (inimigo1) {
		sourceX_inimigo = al_get_bitmap_width(goblin->imagem) / 4;
	}
	else {
		sourceX_inimigo = 0;
	}
	if (sourceX_inimigo >= al_get_bitmap_width(goblin->imagem)) {
		sourceX_inimigo = 0;
	}
	//}


}

void desenha() {


	if (atacando) {
		al_draw_bitmap_region(sprite_atacando->imagem, sourceX_atacando, 0, sprite_atacando->largura / 10, sprite_atacando->altura, personagem->x - 3, personagem->y - 11, j);

	}
	else {
		if (draw) {
			draw = false;

			//frame = al_create_sub_bitmap(personagem->imagem, (personagem->largura / 10) * i, 0, personagem->largura / 10 - 5, personagem->altura);
			al_draw_bitmap_region(personagem->imagem, sourceX, 0, personagem->largura / 10, personagem->altura, personagem->x, personagem->y, j);

		}
		else if (draw2) {
			draw2 = false;
			frame2 = al_create_sub_bitmap(sprite_parado->imagem, (sprite_parado->largura / 11) * i, 0, sprite_parado->largura / 11, sprite_parado->altura);
			al_draw_bitmap(frame2, personagem->x, personagem->y, j);
			al_flip_display();

		}
	}


	if (inimigo1) {
		al_draw_bitmap_region(goblin->imagem, sourceX_inimigo, 0, goblin->largura / 4, goblin->altura, goblin->x, goblin->y, k);


	}
	if (espada_ativa) {
		al_draw_bitmap(espada->imagem, espada->x, espada->y, l);
	}
	if (item_processador) {
		al_draw_bitmap(processador->imagem, processador->x, processador->y, 0);
	}
	if (item_processador_mini) {
		al_draw_bitmap(processador_mini->imagem, processador_mini->x, processador_mini->y, 0);
		
	}
	if (item_placa) {
		al_draw_bitmap(placa_mae->imagem, placa_mae->x, placa_mae->y, 0);
	}
	if (item_placa_mini) {
		al_draw_bitmap(placa_mae_mini->imagem, placa_mae_mini->x, placa_mae_mini->y, 0);
		
	}

	if (inv_placa)
		al_draw_bitmap(placa_mae_inv, 0, 0, 0);
	//aqui!!
	if (inventarioo)
		al_draw_bitmap(inventario->imagem, 0, 0, 0);
	
}


// Main
int main(void) {

	inicialização();

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_flip_display();

	// Carrega imagem
	background = al_load_bitmap("imagens/menu1.jpg");
	background_jogo1 = al_load_bitmap("imagens/background.jpg");
	infoss = al_load_bitmap("imagens/infos.jpg");
	creditoss = al_load_bitmap("imagens/creditos.jpg");

	// teste
	processador_inv = al_load_bitmap("imagens/pc.png");
	placa_mae_inv = al_load_bitmap("imagens/pcmae.png");
	inventario = al_load_bitmap("imagens/inventario.png");
	inventario_processador = al_load_bitmap("imagens/inventario_processador.png");
	inventario_placa_mae = al_load_bitmap("imagens/inventario_placa_mae.png");;
	musica = al_load_sample("musica.ogg");
	fonte = al_load_font("Fontes/arial.ttf", 48, 0);

	songInstance = al_create_sample_instance(musica);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());



	personagem = (Objeto*)malloc(sizeof(Objeto));
	personagem->imagem = al_load_bitmap("Sprites/MC_Sprite_walk.png");
	personagem->largura = 500;
	personagem->altura = 49;
	personagem->y = Chao - personagem->altura;
	personagem->x = 0;
	personagem->vida = 3;

	sprite_parado = (Objeto*)malloc(sizeof(Objeto));
	sprite_parado->imagem = al_load_bitmap("Sprites/MC_Sprite_idle.png");
	sprite_parado->x = personagem->x;
	sprite_parado->largura = 440;
	sprite_parado->altura = 49;
	sprite_parado->y = personagem->y;

	goblin = (Objeto*)malloc(sizeof(Objeto));
	goblin->imagem = al_load_bitmap("Sprites/goblins.png");
	goblin->x = 300;
	goblin->altura = 49;
	goblin->y = Chao - goblin->altura;
	goblin->largura = 246;
	goblin->vida = 3;

	sprite_atacando = (Objeto*)malloc(sizeof(Objeto));
	sprite_atacando->imagem = al_load_bitmap("Sprites/MC_Sprite_attack.png");
	sprite_atacando->largura = 630;
	sprite_atacando->altura = 60;
	sprite_atacando->y = personagem->y;
	sprite_atacando->x = personagem->x;

	espada = (Objeto*)malloc(sizeof(Objeto));
	espada->imagem = al_load_bitmap("Sprites/sword.png");
	espada->largura = 65;
	espada->altura = 16;
	espada->y = personagem->y + 30;
	espada->x = personagem->x + 10;

	//Criando objeto do item
	processador = (Objeto*)malloc(sizeof(Objeto));
	processador->imagem = al_load_bitmap("imagens/processador.jpg");
	processador->largura = 63;
	processador->altura = 57;
	processador->x = 700;
	processador->y = Chao - processador->altura;

	processador_mini = (Objeto*)malloc(sizeof(Objeto));
	processador_mini->imagem = al_load_bitmap("imagens/processador_mini.jpg");
	processador_mini->largura = 36;
	processador_mini->altura = 33;
	processador_mini->x = 50;
	processador_mini->y = 100;

	// Placa Mae
	placa_mae = (Objeto*)malloc(sizeof(Objeto));
	placa_mae->imagem = al_load_bitmap("imagens/placamae.jpg");
	placa_mae->largura = 63;
	placa_mae->altura = 57;
	placa_mae->x = 500;
	placa_mae->y = Chao - placa_mae->altura;

	// Placa Mae Mini
	placa_mae_mini = (Objeto*)malloc(sizeof(Objeto));
	placa_mae_mini->imagem = al_load_bitmap("imagens/placamaemini.jpg");
	placa_mae_mini->largura = 36;
	placa_mae_mini->altura = 33;
	placa_mae_mini->x = 10;
	placa_mae_mini->y = 100;

	
	inventario = (Objeto*)malloc(sizeof(Objeto));
	inventario->imagem = al_load_bitmap("imagens/inventario.png");
	inventario->largura = 640;
	inventario->altura = 480;
	
	

	frame2 = al_create_sub_bitmap(sprite_parado->imagem, (sprite_parado->largura / 11) * i, 0, sprite_parado->largura / 11, sprite_parado->altura);


	// Variaveis de controle de menu
	int menu = 1, jogar = 0, creditos = 0, infos = 0, jogo = 1, tocando = 1, morreu = 0, venceu = 0;


	al_start_timer(timer);

	//Looping principal
	while (jogo == 1) {

		ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);

		al_get_keyboard_state(&key_state);

		// Funcao do menu do jogo
		// Ja começa com 1 pois ela irá controlar os demais whiles, as outras opções


		if (menu == 1) {

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_flip_display();

			if (tocando == 1) {
				al_play_sample_instance(songInstance);
			}
			else if (tocando == 0)
				al_stop_sample_instance(songInstance);

			// Se o evento for um clique do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				// Se o clique foi no botão sair
				if ((evento.mouse.x >= 590 &&
					evento.mouse.x <= 630 && evento.mouse.y <= 42 &&
					evento.mouse.y >= 6)) {

					// Fecha o jogo
					menu = 0;
					jogo = 0;
				}
				// Se for em jogar
				else if ((evento.mouse.x >= 217 &&
					evento.mouse.x <= 400 && evento.mouse.y <= 204 &&
					evento.mouse.y >= 144)) {

					al_stop_sample_instance(songInstance);
					menu = 0;
					// Inicia a função jogar
					jogar = 1;
					al_draw_bitmap(background_jogo1, 0, 0, NULL);
					al_flip_display();
				}
				// Se for em infos
				else if ((evento.mouse.x >= 213 &&
					evento.mouse.x <= 402 && evento.mouse.y <= 298 &&
					evento.mouse.y >= 239)) {

					menu = 0;
					infos = 1;
				}
				// Se for em creditos
				else if ((evento.mouse.x >= 210 &&
					evento.mouse.x <= 405 && evento.mouse.y <= 390 &&
					evento.mouse.y >= 330)) {


					menu = 0;
					creditos = 1;
				}
				// Se for em mutar som
				else if ((evento.mouse.x >= 538 &&
					evento.mouse.x <= 582 && evento.mouse.y <= 44 &&
					evento.mouse.y >= 7)) {


					//mutar e desmutar funcionando
					if (tocando == 1)
						tocando = 0;
					else
						tocando = 1;
				}

			}

		}
		//(personagem->x <= goblin->x + goblin->largura / 4) && (personagem->x + personagem->largura/10 >= goblin->x)
		else if (jogar == 1) {

			if (inimigo1 && (personagem->x <= goblin->x + 20) && (personagem->x + 20 >= goblin->x) && (personagem->y + personagem->altura >= goblin->y) && (personagem->y <= goblin->y + goblin->altura)) {
				jogar = 0;

				morreu = 1;
			}

			if ((personagem->x <= processador->x + processador->largura) && (personagem->x + personagem->largura / 10 >= processador->x) && (personagem->y - personagem->altura >= processador->y - processador->altura)) {
				item_processador = false;
				item_processador_mini = true;

				

			}

			if (inimigo1 && espada_ativa && (espada->x + 50 >= goblin->x) && (espada->x <= goblin->x + 20) && (espada->y >= goblin->y) && (espada->y <= goblin->y + goblin->altura)) {
				espada_ativa = false;
				goblin->vida--;

				//jogar = 0; 
				//venceu = 1;
			}

			if (inimigo1) {
				if (goblin->vida <= 0) {
					inimigo1 = false;
					espada_ativa = false;
					item_processador = true;
					processador->x = goblin->x;
					item_placa = true;
				}
			}

			if ((espada->x >= 620) || (espada->x <= 0)) {
				espada_ativa = false;
			}

			if (!inimigo1 && (personagem->x <= placa_mae->x + placa_mae->largura) && (personagem->x + personagem->largura / 10 >= placa_mae->x) && (personagem->y - personagem->altura >= placa_mae->y - placa_mae->altura)) {

				item_placa = false;
				item_placa_mini = true;
			}

			/*if (atacando) {
				if (j == 1) {
					if ((personagem->x + 50 >= goblin->x+10) && (personagem->y - personagem->altura >= goblin->y - goblin->altura)) {
						inimigo1 = false;
						jogar = 0;
						venceu = 1;
					}
				}
				if (j == 0) {
					if ((personagem->x - 50 <= goblin->x + 10) && (personagem->y - personagem->altura >= goblin->y - goblin->altura)) {
						inimigo1 = false;
						jogar = 0;
						venceu = 1;
					}
				}
			}*/



			if (evento.type == ALLEGRO_EVENT_TIMER) {
				movimentacao(evento);

				al_draw_bitmap(background_jogo1, 0, 0, NULL);

				desenha();

				al_flip_display();
			}

		}
		// Se clicar em creditos
		else if (creditos == 1) {

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(creditoss, 0, 0, 0);
			al_flip_display();

			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				if ((evento.mouse.x >= 590 &&
					evento.mouse.x <= 630 && evento.mouse.y <= 42 &&
					evento.mouse.y >= 6)) {

					creditos = 0;
					menu = 1;
				}
			}
		}
		// Se clicar em infos
		else if (infos == 1) {

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(infoss, 0, 0, 0);
			al_flip_display();

			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				if ((evento.mouse.x >= 590 &&
					evento.mouse.x <= 630 && evento.mouse.y <= 42 &&
					evento.mouse.y >= 6)) {

					infos = 0;
					menu = 1;
				}

			}
		}

		else if (morreu == 1) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 320, 240, ALLEGRO_ALIGN_CENTRE, "Voce Perdeu!");
			al_flip_display();
		}

		else if (venceu == 1) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(fonte, al_map_rgb(0, 0, 0), 320, 240, ALLEGRO_ALIGN_CENTRE, "Voce Venceu!");
			al_flip_display();
		}

		// Se for no x da janela
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			jogo = 0;
		}

	}

	al_destroy_display(janela);
	al_destroy_bitmap(background);
	al_destroy_bitmap(creditoss);
	al_destroy_bitmap(infoss);
	al_destroy_bitmap(frame);
	al_destroy_bitmap(frame2);
	al_destroy_bitmap(goblin->imagem);
	al_destroy_bitmap(personagem->imagem);
	al_destroy_bitmap(sprite_atacando->imagem);
	al_destroy_bitmap(sprite_parado->imagem);
	al_destroy_bitmap(processador_mini->imagem);
	al_destroy_bitmap(processador->imagem);
	al_destroy_event_queue(fila_eventos);
	al_destroy_font(fonte);

	free(personagem);
	free(sprite_atacando);
	free(sprite_parado);
	free(goblin);
	free(espada);
	free(processador_mini);

	return 0;
}