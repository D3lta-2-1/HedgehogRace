typedef struct {
    int id;
    int line;
    int column;
} Hedgehog;

typedef struct {
    Hegdehog hedgehogs[MAX_HEGDEHOG_COUNT];
} Player;

typedef struct {
    Board* board;
    Player* players;
    uint8_t current_player;
} Game;

bool can_move_vertically(Game* g, int hedgehog_index);