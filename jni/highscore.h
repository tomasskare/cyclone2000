#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#define HS_NUM 10
#define HS_COL_POS 0
#define HS_COL_NAME 1
#define HS_COL_SCORE 2
#define HS_COL_STAGE 3
#define HS_COL_NUM 4

#define HS_NAME_MAX 8

typedef struct hs_entry_s {
  char name[HS_NAME_MAX+1];
  int score;
  int stage;
} hs_entry_t;

typedef struct hs_lines_s {
  line_t *lines[HS_NUM][HS_COL_NUM];
} hs_lines_t;

extern hs_entry_t hs_entries[HS_NUM];

void highscore_line_build(hs_lines_t *hs_lines);
void highscore_file_read(void);
void highscore_file_write(void);
int highscore_store(char *name, int score, int stage);
void highscore_reset(void);
int highscore_is_dirty(void);

void highscore_init(int hspos);
int highscore_update(uint64_t msec);
void highscore_draw(int viewmode);
void highscore_control(int choice, int data2, int data3);

#endif
