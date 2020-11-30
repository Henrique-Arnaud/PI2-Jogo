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
#define FrameFps 25.0
#define InimigoFps 11.0
#define Chao 420


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

Objeto* personagem, * sprite_parado, * goblin, * goblin2, * espada, * processador, * processador_mini, * placa_mae, * placa_mae_mini, * sprite_atacando, * inventario;
Objeto* placa_de_video, * placa_de_video_mini, * memoria_ram, * memoria_ram_mini, * portal, * inimigo1_mapa2, * inimigo2_mapa2, * boss;

ALLEGRO_FONT* fonte = NULL;
ALLEGRO_BITMAP* frame = NULL;
ALLEGRO_BITMAP* frame2 = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_TIMER* frametimer = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* background_jogo1 = NULL;
ALLEGRO_BITMAP* chao = NULL;
ALLEGRO_BITMAP* chao_fim = NULL;
ALLEGRO_BITMAP* plataforma1 = NULL;
ALLEGRO_BITMAP* plataforma2 = NULL;
ALLEGRO_BITMAP* plataforma3 = NULL;
ALLEGRO_BITMAP* infoss = NULL;
ALLEGRO_BITMAP* creditoss = NULL;
ALLEGRO_SAMPLE* musica = NULL;

ALLEGRO_SAMPLE_INSTANCE* songInstance = NULL;
ALLEGRO_BITMAP* placa_mae_inv = NULL;
ALLEGRO_BITMAP* processador_inv = NULL;
ALLEGRO_BITMAP* inventario_processador = NULL;
ALLEGRO_BITMAP* inventario_placa_mae = NULL;
ALLEGRO_BITMAP* inventario_placa_de_video = NULL;

ALLEGRO_BITMAP* desc_placa_mae = NULL;
ALLEGRO_BITMAP* desc_processador = NULL;
ALLEGRO_BITMAP* desc_placa_de_video = NULL;
ALLEGRO_BITMAP* desc_memoria_ram = NULL;

ALLEGRO_SAMPLE* clique_menu = NULL;
ALLEGRO_SAMPLE* som_espada = NULL;
ALLEGRO_SAMPLE* pegar_item = NULL;

int pressionadox = 0;
int i = 0;
int aux = 0;
int j = 0;
int k = 0;
int l = 0;
int lado;
int lado_inimigo1;
int lado_inimigo2;

bool draw = false, draw2 = true, ativo = false, item_processador = false, item_processador_mini = false, item_placa = false, item_placa_mini = false;
bool item_placa_de_video = false, item_placa_de_video_mini = false, item_memoria_ram = false, item_memoria_ram_mini = false;
bool descricao_processador, descricao_placa_mae, descricao_placa_de_video, descricao_memoria_ram;
//teste
bool inv_placa = false;
bool inv_processador = false;
bool inventarioo = false;
bool portal_ativo = false;

//variaveis do mapa

int colunas_mapa1 = 11;
int linhas_mapa1 = 9;
int tileSize = 59;
int mapa_atual = 1;

int map1[9][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 7, 8, 8, 9, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				  {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3} };

int map2[9][11] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 7, 8, 8, 8, 9},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 7, 8, 9, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				  {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3} };


int pressionando = 0;
float velocidade_movimento = 4.5;
float velx;
float vely;
float pLimite;

velx = 0;
vely = 0;

atacando = false, espada_ativa = false;
float velocidade_pulo = 18;
int caindo = 1;
int pulando = 0;

const float gravidade = 0.80;

int sourceX = 0, sourceX_inimigo = 0, sourceX_atacando;

bool inimigo1 = true, inimigo2 = true, inimigo3 = false, inimigo4 = false;
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
	frametimer = al_create_timer(1.0 / FrameFps);

	janela = al_create_display(LARGURA, ALTURA);

	fila_eventos = al_create_event_queue();

	al_set_window_title(janela, "Em busca do Hardware Perdido");

	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_eventos, al_get_timer_event_source(frametimer));

}


int colisao(float ax, float ay, float bx, float by, int aw, int ah, int bw, int bh, int limite) {
	if (ax + aw > bx && ax < bx + limite + bw && ay + ah > by && ay < by + bh) {
		return 1;
	}
	else {
		return 0;
	}
}


// Função movimentação
void movimentacao(ALLEGRO_EVENT evento) {


	if (evento.timer.source == timer) {

		ativo = true;

		pressionando = 0;

		if (al_key_down(&key_state, ALLEGRO_KEY_UP) && pulando == 0 && caindo == 0) {
			pulando = 1;
			printf("%d\n\n", pulando);
			pLimite = personagem->y;

		}
		if (al_key_down(&key_state, ALLEGRO_KEY_Z) && !atacando && !espada_ativa) {
			atacando = true;
			al_play_sample(som_espada, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
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

		if (pulando == 1 && personagem->y > pLimite - 35) {
			//pulando = 0;
			personagem->y += vely;
			vely = -velocidade_pulo;
			caindo = 1;
		}
		else if (caindo == 1) {
			pulando = 0;
			vely += gravidade;
			personagem->y += vely;
		}
		else {
			vely = 0;
		}

		/*if (!pulando) {
			vely += gravidade;
		}
		else {
			vely = 0;
		}*/
		if (personagem->x >= -1 && personagem->x + personagem->largura / 10 <= 639) {
			personagem->x += velx;
		}
		if (personagem->x <= -1) {
			personagem->x = 0;
		}
		else if (personagem->x + personagem->largura / 10 >= 639) {
			personagem->x = 638 - personagem->largura / 10;
		}


		/*if (pulando) {
			personagem->y = Chao - 49;
		}*/

		if (inimigo1 && personagem->x - goblin->x < 0) {
			goblin->x -= velocidade_inimigo;
			k = 0;
		}
		else if (inimigo1 && personagem->x - goblin->x > 0) {
			goblin->x += velocidade_inimigo;
			k = 1;
		}

		if (inimigo2 && lado == 0) {
			goblin2->x -= velocidade_inimigo;
			if (goblin2->x <= tileSize * 4 + 5) {
				lado = 1;
			}
		}
		if (inimigo2 && lado == 1) {
			goblin2->x += velocidade_inimigo;
			if (goblin2->x >= tileSize * 4 + tileSize * 4 - 50) {
				lado = 0;
			}
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


	if (evento.timer.source == frametimer) {

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
	}

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

void desenha(ALLEGRO_EVENT evento) {

	
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
	if (inimigo2) {
		al_draw_bitmap_region(goblin2->imagem, sourceX_inimigo, 0, goblin2->largura / 4, goblin2->altura, goblin2->x, goblin2->y, lado);
	}
	if (inimigo3) {
		al_draw_bitmap(inimigo1_mapa2->imagem, inimigo1_mapa2->x, inimigo1_mapa2->y, lado_inimigo1);
	}
	if (inimigo4) {
		al_draw_bitmap(inimigo2_mapa2->imagem, inimigo2_mapa2->x, inimigo2_mapa2->y, lado_inimigo2);
	}
	if (espada_ativa) {
		al_draw_bitmap(espada->imagem, espada->x, espada->y, l);
	}
	if (item_processador) {
		al_draw_bitmap(processador->imagem, processador->x, processador->y, 0);
	}
	if (item_processador_mini) {
		al_draw_bitmap(processador_mini->imagem, processador_mini->x, processador_mini->y, 0);
		inventario->imagem = inventario_processador;

	}
	if (item_placa) {
		al_draw_bitmap(placa_mae->imagem, placa_mae->x, placa_mae->y, 0);
	}
	if (item_placa_mini) {
		al_draw_bitmap(placa_mae_mini->imagem, placa_mae_mini->x, placa_mae_mini->y, 0);
		inventario->imagem = inventario_placa_mae;
	}
	/*if (item_placa_de_video) {
		al_draw_bitmap(placa_de_video->imagem, placa_de_video->x, placa_de_video->y, 0);

	}
	if (item_placa_de_video_mini) {
		al_draw_bitmap(placa_de_video_mini->imagem, placa_de_video_mini->x, placa_de_video_mini->y, 0);
		inventario->imagem = inventario_placa_de_video;
	}
	if (item_memoria_ram) {
		al_draw_bitmap(memoria_ram->imagem, memoria_ram->x, memoria_ram->y, 0);
	}
	if (item_memoria_ram_mini) {
		al_draw_bitmap(memoria_ram_mini->imagem, memoria_ram_mini->x, memoria_ram_mini->y, 0);
	}*/

	if (inv_placa)
		al_draw_bitmap(placa_mae_inv, 0, 0, 0);
	//aqui!!
	if (inventarioo)
		al_draw_bitmap(inventario->imagem, 0, 0, 0);
	if (descricao_processador) {
		al_draw_bitmap(desc_processador, 0, 0, 0);
	}
	if (descricao_placa_mae) {
		al_draw_bitmap(desc_placa_mae, 0, 0, 0);
	}
	if (descricao_placa_de_video) {
		al_draw_bitmap(desc_placa_de_video, 0, 0, 0);
	}
	if (descricao_memoria_ram) {
		al_draw_bitmap(desc_memoria_ram, 0, 0, 0);
	}
	if (portal_ativo) {
		al_draw_bitmap(portal->imagem, portal->x, 150, 0);
	}

}

void desenhar_mapa(int mapa[9][11]) {

	int cont_i, cont_j;

	for (cont_i = 0; cont_i < linhas_mapa1; cont_i++) {
		for (cont_j = 0; cont_j < colunas_mapa1; cont_j++) {
			if (mapa[cont_i][cont_j] == 0) {
	
			}
			else if (mapa[cont_i][cont_j] == 2) {
				al_draw_bitmap_region(chao, tileSize * 0, tileSize * 0, tileSize, tileSize, cont_j * tileSize, cont_i * tileSize, 0);
			}
			else if (mapa[cont_i][cont_j] == 3) {
				al_draw_bitmap_region(chao_fim, tileSize * 0, tileSize * 0, tileSize, tileSize, cont_j * tileSize, cont_i * tileSize, 0);
			}
			else if (mapa[cont_i][cont_j] == 7) {
				al_draw_bitmap_region(plataforma1, tileSize * 0, tileSize * 0, tileSize, 44, cont_j * tileSize, cont_i * tileSize, 0);
			}
			else if (mapa[cont_i][cont_j] == 8) {
				al_draw_bitmap_region(plataforma2, tileSize * 0, tileSize * 0, tileSize, 44, cont_j * tileSize, cont_i * tileSize, 0);
			}
			else if (mapa[cont_i][cont_j] == 9) {
				al_draw_bitmap_region(plataforma3, tileSize * 0, tileSize * 0, tileSize, 44, cont_j * tileSize, cont_i * tileSize, 0);
			}
		}
	}
}


// Main
int main(void) {

	inicialização();
	printf("Oi gustavo");
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_flip_display();

	// Carrega imagem
	background = al_load_bitmap("imagens/menu1.jpg");
	background_jogo1 = al_load_bitmap("imagens/background.jpg");
	chao = al_load_bitmap("sprites/chao.jpg");
	chao_fim = al_load_bitmap("sprites/chao_fim.jpg");
	plataforma1 = al_load_bitmap("sprites/plataforma1.png");
	plataforma2 = al_load_bitmap("sprites/plataforma2.png");
	plataforma3 = al_load_bitmap("sprites/plataforma3.png");
	infoss = al_load_bitmap("imagens/infos.jpg");
	creditoss = al_load_bitmap("imagens/creditos.jpg");
	inventario_placa_de_video = al_load_bitmap("imagens/inventario_placa_de_video.png");
	// teste
	processador_inv = al_load_bitmap("imagens/pc.png");
	placa_mae_inv = al_load_bitmap("imagens/pcmae.png");
	inventario = al_load_bitmap("imagens/inventario.png");
	inventario_processador = al_load_bitmap("imagens/inventario_processador.png");
	inventario_placa_mae = al_load_bitmap("imagens/inventario_placa_mae.png");;
	musica = al_load_sample("musica.ogg");
	fonte = al_load_font("Fontes/arial.ttf", 48, 0);

	pegar_item = al_load_sample("pegar_item.wav");
	clique_menu = al_load_sample("clique_menu.wav");//Som de clique
	som_espada = al_load_sample("som_espada.wav");


	desc_processador = al_load_bitmap("imagens/desc_processador.png");
	desc_placa_mae = al_load_bitmap("imagens/desc_placa_mae.png");
	desc_memoria_ram = al_load_bitmap("imagens/desc_memoria_ram.png");
	desc_placa_de_video = al_load_bitmap("imagens/desc_placa_de_video.png");

	songInstance = al_create_sample_instance(musica);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	//Carregando sons
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);//Reservando canais de audio no mixer principal



	personagem = (Objeto*)malloc(sizeof(Objeto));
	personagem->imagem = al_load_bitmap("Sprites/MC_Sprite_walk.png");
	personagem->largura = 500;
	personagem->altura = 49;
	personagem->y = 0;
	personagem->x = 20;
	personagem->vida = 3;

	sprite_parado = (Objeto*)malloc(sizeof(Objeto));
	sprite_parado->imagem = al_load_bitmap("Sprites/MC_Sprite_idle.png");
	sprite_parado->x = personagem->x;
	sprite_parado->largura = 440;
	sprite_parado->altura = 49;
	sprite_parado->y = personagem->y;

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	goblin = (Objeto*)malloc(sizeof(Objeto));
	goblin->imagem = al_load_bitmap("Sprites/goblins.png");
	goblin->x = 300;
	goblin->altura = 49;
	goblin->y = Chao - goblin->altura;
	goblin->largura = 246;
	goblin->vida = 5;

	goblin2 = (Objeto*)malloc(sizeof(Objeto));
	goblin2->imagem = al_load_bitmap("Sprites/goblins.png");
	goblin2->x = tileSize * 5;
	goblin2->altura = 49;
	goblin2->y = tileSize * 4 - goblin2->altura;
	goblin2->largura = 246;
	goblin2->vida = 4;

	inimigo1_mapa2 = (Objeto*)malloc(sizeof(Objeto));
	inimigo1_mapa2->imagem = al_load_bitmap("Sprites/inimigo2.png");
	inimigo1_mapa2->x = tileSize * 7;
	inimigo1_mapa2->altura = 90;
	inimigo1_mapa2->y = tileSize * 3 - inimigo1_mapa2->altura;
	inimigo1_mapa2->largura = 41;
	inimigo1_mapa2->vida = 4;

	inimigo2_mapa2 = (Objeto*)malloc(sizeof(Objeto));
	inimigo2_mapa2->imagem = al_load_bitmap("Sprites/inimigo2.png");
	inimigo2_mapa2->x = tileSize * 5;
	inimigo2_mapa2->altura = 90;
	inimigo2_mapa2->y = Chao - inimigo2_mapa2->altura;
	inimigo2_mapa2->largura = 41;
	inimigo2_mapa2->vida = 4;

	boss = (Objeto*)malloc(sizeof(Objeto));
	boss->imagem = al_load_bitmap("Sprites/boss.png");
	boss->x = tileSize * 5;
	boss->altura = 49;
	boss->y = tileSize * 4 - goblin2->altura;
	boss->largura = 246;
	boss->vida = 4;

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

	// Inventario
	inventario = (Objeto*)malloc(sizeof(Objeto));
	inventario->imagem = al_load_bitmap("imagens/inventario.png");
	inventario->largura = 640;
	inventario->altura = 480;


	portal = (Objeto*)malloc(sizeof(Objeto));
	portal->imagem = al_load_bitmap("Sprites/portal.png");
	portal->largura = 57;
	portal->altura = 150;
	portal->x = 570;
	portal->y = 0;


	// Placa de video
	/*placa_de_video = (Objeto*)malloc(sizeof(Objeto));
	placa_de_video->imagem = al_load_bitmap("imagens/placa_de_video.png");
	placa_de_video->largura = 63;
	placa_de_video->altura = 57;
	placa_de_video->x = 300;
	placa_de_video->y = Chao - placa_de_video->altura;
	// Placa de video mini
	placa_de_video_mini = (Objeto*)malloc(sizeof(Objeto));
	placa_de_video_mini->imagem = al_load_bitmap("imagens/placa_de_video_mini.png");
	placa_de_video_mini->largura = 36;
	placa_de_video_mini->altura = 33;
	placa_de_video_mini->x = 90;
	placa_de_video_mini->y = 100;
	// Memoria ram
	memoria_ram = (Objeto*)malloc(sizeof(Objeto));
	memoria_ram->imagem = al_load_bitmap("imagens/memoria_ram.png");
	memoria_ram->largura = 63;
	memoria_ram->altura = 57;
	memoria_ram->x = 300;
	memoria_ram->y = 100;
	// Memoria ram MINI

	memoria_ram_mini = (Objeto*)malloc(sizeof(Objeto));
	memoria_ram_mini->imagem = al_load_bitmap("imagens/memoria_ram_mini.png");
	memoria_ram_mini->largura = 36;
	memoria_ram_mini->altura = 33;
	memoria_ram_mini->x = 140;
	memoria_ram_mini->y = 100;*/

	frame2 = al_create_sub_bitmap(sprite_parado->imagem, (sprite_parado->largura / 11) * i, 0, sprite_parado->largura / 11, sprite_parado->altura);


	// Variaveis de controle de menu
	int menu = 1, jogar = 0, creditos = 0, infos = 0, jogo = 1, tocando = 1, morreu = 0, venceu = 0;


	al_start_timer(timer);
	al_start_timer(frametimer);

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
					al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
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
					al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					menu = 0;
					infos = 1;
				}
				// Se for em creditos
				else if ((evento.mouse.x >= 210 &&
					evento.mouse.x <= 405 && evento.mouse.y <= 390 &&
					evento.mouse.y >= 330)) {

					al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					menu = 0;
					creditos = 1;
				}
				// Se for em mutar som
				else if ((evento.mouse.x >= 538 &&
					evento.mouse.x <= 582 && evento.mouse.y <= 44 &&
					evento.mouse.y >= 7)) {


					//mutar e desmutar funcionando
					if (tocando == 1) {
						al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						tocando = 0;
					}

					else {
						al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						tocando = 1;
					}

				}

			}

		}

		else if (jogar == 1) {

			printf("mapa:  %d\n\n", mapa_atual);

			if (mapa_atual == 1 && !colisao(personagem->x, personagem->y, 0, Chao, personagem->largura / 10, personagem->altura, tileSize, tileSize, 640) == 1 && !colisao(personagem->x, personagem->y, tileSize * 4 + 30, tileSize * 4, personagem->largura / 10, personagem->altura, tileSize * 4 - 50, 0, 0)) {
				caindo = 1;
			}
			else if (mapa_atual == 2 && !colisao(personagem->x, personagem->y, 0, Chao, personagem->largura / 10, personagem->altura, tileSize, tileSize, 640) == 1) {
				caindo = 1;
			}
			else {
				caindo = 0;
				pulando = 0;
			}

			if (colisao(personagem->x, personagem->y, 0, Chao, personagem->largura / 10, personagem->altura, tileSize, tileSize, 640) == 1) {
				personagem->y = Chao - personagem->altura;
			}
			if (mapa_atual == 1 && colisao(personagem->x, personagem->y, tileSize * 4 + 30, tileSize * 4, personagem->largura / 10, personagem->altura, tileSize * 4 - 50, 0, 0)) {
				personagem->y = tileSize * 4 - personagem->altura;
			}

			//se o GOBLIN encostar no PLAYER ou vice-versa
			if (inimigo1 && (personagem->x <= goblin->x + 20) && (personagem->x + 20 >= goblin->x) && (personagem->y + personagem->altura >= goblin->y) && (personagem->y <= goblin->y + goblin->altura)) {
				jogar = 0;
				morreu = 1;
			}
			if (inimigo2 && (personagem->x <= goblin2->x + 20) && (personagem->x + 20 >= goblin2->x) && (personagem->y + personagem->altura >= goblin2->y) && (personagem->y <= goblin2->y + goblin2->altura)) {
				jogar = 0;
				morreu = 1;
			}

			if ((personagem->x <= processador->x + processador->largura) && (personagem->x + personagem->largura / 10 >= processador->x) && (personagem->y - personagem->altura >= processador->y - processador->altura)) {
				al_play_sample(pegar_item, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				processador->y = 1000, processador->x = 1000;
				item_processador = false;
				item_processador_mini = true;

			}

			if (inimigo1 && espada_ativa && (espada->x + 50 >= goblin->x) && (espada->x <= goblin->x + 20) && (espada->y >= goblin->y) && (espada->y <= goblin->y + goblin->altura)) {
				espada_ativa = false;
				goblin->vida--;
				if (k == 0) {
					goblin->x += 10;
				}
				else if (k == 1) {
					goblin->x -= 10;
				}
			}

			if (inimigo1) {

				if (goblin->vida <= 0) {
					inimigo1 = false;
					espada_ativa = false;
					item_processador = true;
					processador->x = goblin->x;
				}
			}

			if (inimigo2 && espada_ativa && (espada->x + 50 >= goblin2->x) && (espada->x <= goblin2->x + 20) && (espada->y >= goblin2->y) && (espada->y <= goblin2->y + goblin2->altura)) {
				espada_ativa = false;
				goblin2->vida--;
				if (lado == 0) {
					goblin2->x += 10;
				}
				else if (lado == 1) {
					goblin2->x -= 10;
				}
			}

			if (inimigo2) {

				if (goblin2->vida <= 0) {
					inimigo2 = false;
					espada_ativa = false;
					item_placa = true;
					placa_mae->x = goblin2->x;
					placa_mae->y = goblin2->y;
				}
			}

			if ((espada->x >= 620) || (espada->x <= 0)) {
				espada_ativa = false;
			}

			if (!inimigo1 && (personagem->x <= placa_mae->x + placa_mae->largura) && (personagem->x + personagem->largura / 10 >= placa_mae->x) && (personagem->y - personagem->altura >= placa_mae->y - placa_mae->altura)) {

				al_play_sample(pegar_item, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				placa_mae->y = 1000, placa_mae->x = 1000;
				item_placa = false;

				item_placa_mini = true;
			}

			if (!inimigo1 && !inimigo2 && item_placa_mini && item_processador_mini && mapa_atual == 1) {
				portal_ativo = true;
				if (personagem->x + personagem->largura / 10 >= portal->x && personagem->x <= portal->x + portal->largura && personagem->y + personagem->altura > portal->y) {
					al_rest(3);
					portal_ativo = false;
					mapa_atual = 2;
					personagem->x = 20;
					personagem->y = 75;
				}
			}

			if (mapa_atual == 2) {

				if (inimigo3 && espada_ativa && (espada->x + 50 >= inimigo1_mapa2->x) && (espada->x <= inimigo1_mapa2->x + 20) && (espada->y >= inimigo1_mapa2->y) && (espada->y <= inimigo1_mapa2->y + inimigo1_mapa2->altura)) {
					espada_ativa = false;
					inimigo1_mapa2->vida--;
					if (lado_inimigo1 == 0) {
						inimigo1_mapa2->x += 10;
					}
					else if (lado_inimigo1 == 1) {
						inimigo1_mapa2->x -= 10;
					}
				}
				if (inimigo4 && espada_ativa && (espada->x + 50 >= inimigo2_mapa2->x) && (espada->x <= inimigo2_mapa2->x + 20) && (espada->y >= inimigo2_mapa2->y) && (espada->y <= inimigo2_mapa2->y + inimigo2_mapa2->altura)) {
					espada_ativa = false;
					inimigo2_mapa2->vida--;
					if (lado_inimigo2 == 0) {
						inimigo2_mapa2->x += 10;
					}
					else if (lado_inimigo2 == 1) {
						inimigo2_mapa2->x -= 10;
					}
				}
			}

			if (evento.type == ALLEGRO_EVENT_TIMER) {

				movimentacao(evento);

				if (pressionando == 1 || pressionando == 0) {
					if (evento.timer.source == frametimer) {

						al_draw_bitmap(background_jogo1, 0, 0, NULL);
						if (mapa_atual == 1) {
							desenhar_mapa(map1);
						}
						if (mapa_atual == 2) {
							desenhar_mapa(map2);
						}
					}
					desenha(evento);


					al_flip_display();

				}
				else {
					al_draw_bitmap(background_jogo1, 0, 0, NULL);

					desenha(evento);

					if (mapa_atual == 1) {
						desenhar_mapa(map1);
					}
					if (mapa_atual == 2) {
						desenhar_mapa(map2);
					}

					al_flip_display();
				}
			}
			if (inventarioo) {

				if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

					if ((evento.mouse.x >= 122 &&
						evento.mouse.x <= 211 && evento.mouse.y <= 299 &&
						evento.mouse.y >= 209)) {

						descricao_processador = true;
					}
					if ((evento.mouse.x >= 227 &&
						evento.mouse.x <= 317 && evento.mouse.y <= 299 &&
						evento.mouse.y >= 219)) {

						descricao_placa_mae = true;
					}
					if ((evento.mouse.x >= 327 &&
						evento.mouse.x <= 416 && evento.mouse.y <= 302 &&
						evento.mouse.y >= 210)) {

						descricao_placa_de_video = true;
					}
					if ((evento.mouse.x >= 427 &&
						evento.mouse.x <= 517 && evento.mouse.y <= 300 &&
						evento.mouse.y >= 209)) {

						descricao_memoria_ram = true;
					}



				}
				else if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
					descricao_processador = false;
					descricao_placa_mae = false;
					descricao_placa_de_video = false;
					descricao_memoria_ram = false;
				}
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

					al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
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

					al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					infos = 0;
					menu = 1;
				}

			}

		}





		// Se for no x da janela
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			jogo = 0;
		}

	}

	al_destroy_display(janela);
	al_destroy_bitmap(background);
	al_destroy_bitmap(chao);
	al_destroy_bitmap(chao_fim);
	al_destroy_bitmap(plataforma1);
	al_destroy_bitmap(creditoss);
	al_destroy_bitmap(infoss);
	al_destroy_bitmap(frame);
	al_destroy_bitmap(frame2);
	al_destroy_bitmap(personagem->imagem);
	al_destroy_bitmap(sprite_atacando->imagem);
	al_destroy_bitmap(sprite_parado->imagem);
	al_destroy_bitmap(processador_mini->imagem);
	al_destroy_bitmap(processador->imagem);
	al_destroy_event_queue(fila_eventos);
	al_destroy_font(fonte);
	al_destroy_bitmap(inventario->imagem);
	al_destroy_bitmap(goblin->imagem);
	al_destroy_timer(timer);
	al_destroy_timer(frametimer);

	free(goblin);
	free(personagem);
	free(sprite_atacando);
	free(sprite_parado);
	free(espada);
	free(processador_mini);
	free(processador);
	free(placa_de_video);
	free(placa_de_video_mini);
	free(placa_mae);
	free(placa_mae_mini);
	free(inventario);
	free(memoria_ram);
	free(memoria_ram_mini);


	return 0;
}