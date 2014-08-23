//Proyecto Allegro - Interfaz Gráfica
//Pac-Man - Guillermo Iglesias Birkner
#include <allegro.h>
#include <cstdlib>

#define LARGO 880
#define ALTO 600
#define MAXFILAS 20
#define MAXCOLS 31
#define VELOCIDAD 70

BITMAP *buffer, *roca, *pacbmp, *pacman, *comida, *muerte;
SAMPLE *chomp, *death;

//Pacman
int dir = 4;
int px = 30*14, py = 30*17;
int anteriorpx, anteriorpy;

char mapa[MAXFILAS][MAXCOLS] = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X    | o o  XXXXX  o o |    X",
    "X XXX XXXXX XXXXX XXXXX XXX X",
    "X XXX XXXXX XXXXX XXXXX XXX X",
    "X    |  |           |  |    X",
    "X XXX XX XXXXXXXXXXX XX XXX X",
    "X    |XX     XXX     XX|    X",
    "X XXX XXXXXX XXX XXXXXX XXX X",
    "X XXX XX o  |   |  o XX XXX X",
    "     |XX XXXXXXXXXXX XX|     ",
    "X XXX XX XXXXXXXXXXX XX XXX X",
    "X XXX XX o  |   |  o XX XXX X",
    "X XXX XXXXXX XXX XXXXXX XXX X",
    "X     XX     XXX     XX     X",
    "X XXX XX XXXXXXXXXXX XX XXX X",
    "X XXX|                 |XXX X",
    "X XXX XXX XXXXXXXXX XXX XXX X",
    "X XXX XXX           XXX XXX X",
    "X    |   |XXXXXXXXX|   |    X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

void dibujar_mapa()
{
    int row,col;
    for (row = 0; row < MAXFILAS; row++)
        for(col = 0; col< MAXCOLS; col++)
        {
            if (mapa[row][col]=='X')
                draw_sprite(buffer, roca, col*30, row*30);
            else if (mapa[row][col]=='o')
            {
                draw_sprite(buffer, comida, col*30, row*30);
                if(py/30 == row && px/30 == col)
                {
                    play_sample(chomp, 100,150,1000,0);
                    mapa[row][col] = ' ';
                }

            }
        }
}

void pantalla()
{
    blit(buffer, screen, 0,0, 0,0, LARGO,ALTO);
}

void dibujar_personaje()
{
    blit(pacbmp, pacman, dir*33,0, 0,0, 33,33);
    draw_sprite(buffer,pacman,px,py);
}

bool game_over()
{
    int row,col;
    for (row = 0; row < MAXFILAS; row++)
        for(col = 0; col< MAXCOLS; col++)
            if (mapa[row][col]=='o') return true;
    return false;
}

class fantasma
{
        BITMAP *enemigo, *enemigobmp;
        int fdir;
        int fx, fy;
    public:
        fantasma(int x, int y); //constructor
        void dibujar_fantasma();
        void mover_fantasma();
        void choque_pacman();
};
fantasma::fantasma(int x, int y)
{
    fx = x;
    fy = y;
    fdir = rand()%4;
    enemigo = create_bitmap (30,30);
    enemigobmp = load_bitmap ("enemigo.bmp",NULL);
}
void fantasma::dibujar_fantasma()
{
    blit(enemigobmp, enemigo,0,0,0,0,30,30);
    draw_sprite(buffer,enemigo,fx,fy);
}
void fantasma::choque_pacman()
{
    if ( py == fy && px == fx || fy == anteriorpy && fx == anteriorpx )
    {
        play_sample(death, 100,150,1000,0);
        for (int i=0; i<=5; i++)
        {
            clear(pacman);
            clear(buffer);
            dibujar_mapa();

            blit(muerte, pacman, i*33, 0,0,0,33,33);
            draw_sprite(buffer, pacman,px,py);

            pantalla();
            rest (80);
        }

        px = 30*14;
        py = 30*17;
        dir = 4;
    }
}
void fantasma::mover_fantasma()
{
    dibujar_fantasma();
    choque_pacman();

    if ( mapa[fy/30][fx/30] == '|' ) fdir = rand()%4;
    if ( fdir == 0 ) { if (mapa[fy/30][(fx-30)/30] != 'X') fx -= 30; else fdir = rand()%4; }
    if ( fdir == 1 ) { if (mapa[fy/30][(fx+30)/30] != 'X') fx += 30; else fdir = rand()%4; }
    if ( fdir == 2 ) { if (mapa[(fy-30)/30][fx/30] != 'X') fy -= 30; else fdir = rand()%4; }
    if ( fdir == 3 ) { if (mapa[(fy+30)/30][fx/30] != 'X') fy += 30; else fdir = rand()%4; }

    //Atajos
    if ( fx <= -30 )     { fx = 870; }
    else if ( fx >= 870) { fx = -30; }
}

int main()
{
    //Iniciación Allegro
    allegro_init();                                                         //Inicia Allegro
    install_keyboard();                                                     //Inicia Teclado
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);                     //Inicia Sonido (Existirá error si el sistema no soporta)
    set_volume(70,70);                                                      //Ajuste de Volumen
    set_color_depth(32);                                                    //Inicia configuracion de colores
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGO, ALTO, 0 ,0);               //Inicia Pantalla segun LARGO y ALTO, windowed

    //Carga de Bitmaps
    buffer = create_bitmap(LARGO,ALTO);                             //Crea el bitmap de la pantalla
    roca = load_bitmap ("roca.bmp", NULL);
    pacbmp = load_bitmap ("pacman.bmp",NULL);
    pacman = create_bitmap (33,33);
    comida = load_bitmap ("comida.bmp",NULL);
    muerte = load_bitmap ("muerte.bmp",NULL);
    chomp = load_sample ("pacman_chomp.wav");
    death = load_sample ("pacman_death.wav");

    fantasma A(30*2,30*3);
    fantasma B(30*15,30*15);
    fantasma C(30*2,30*3);
    fantasma D(30*15,30*15);
    fantasma E(30*2,30*3);
    fantasma F(30*15,30*15);
    fantasma G(30*2,30*3);
    fantasma H(30*15,30*15);

    //play_midi(nombre,1);

    while(!key[KEY_ESC] && game_over() )
    {
        anteriorpx = px;
        anteriorpy = py;

        //Comando de dirección
        if (key[KEY_RIGHT])    { dir = 1; }
        else if (key[KEY_LEFT]){ dir = 0; }
        else if (key[KEY_UP])  { dir = 2; }
        else if (key[KEY_DOWN]){ dir = 3; }

        //Órdenes de dirección
        if ( dir == 0 ) { if (mapa[py/30][(px-30)/30]!= 'X') px -= 30; else dir=4; } //Derecha
        if ( dir == 1 ) { if (mapa[py/30][(px+30)/30]!= 'X') px += 30; else dir=4; } //Izquierda
        if ( dir == 2 ) { if (mapa[(py-30)/30][px/30]!= 'X') py -= 30; else dir=4; } //Arriba
        if ( dir == 3 ) { if (mapa[(py+30)/30][px/30]!= 'X') py += 30; else dir=4; } //Abajo

        //Atajos
        if ( px <= -30 )     { px = 870; }
        else if ( px >= 870) { px = -30; }

        clear(buffer);
        dibujar_mapa();
        dibujar_personaje();
        A.mover_fantasma();
        B.mover_fantasma();
        C.mover_fantasma();
        D.mover_fantasma();
        E.mover_fantasma();
        F.mover_fantasma();
        G.mover_fantasma();
        H.mover_fantasma();
        pantalla();
        rest(VELOCIDAD);

        clear(pacman);
        blit(pacbmp, pacman, 4*33,0, 0,0, 33,33);
        draw_sprite(buffer,pacman,px,py);
        pantalla();
        rest(90);

    }

    //Destrucción de Bitmaps
    destroy_bitmap(buffer);
    destroy_bitmap(roca);
    destroy_bitmap(pacbmp);
    destroy_bitmap(pacman);

    return 0;
}
END_OF_MAIN();
