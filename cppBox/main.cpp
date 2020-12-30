#include <iostream>
#include <iomanip>

#include <chrono>
#include <thread>

#include <vector>
#include <string>
#include <deque>
#include <algorithm>

#include <random>


constexpr size_t HEIGHT = 48;
constexpr size_t WIDTH = 210;
constexpr int64_t SLEEP_PER_FRAME = 10;

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<> distribution(1, 100);
constexpr size_t SNOWFLAKE_CHANCE = 3;


std::string make_snow_row()
{
  std::string row(WIDTH, ' ');

  for (auto& symb : row) {
    if (distribution(generator) < SNOWFLAKE_CHANCE) {
      symb = '*';
    }
  }

  return row;
}

void init_snow(std::vector<std::string>& snow)
{
  for (auto& row : snow) {
    row = make_snow_row();
  }
}

void init_tree(std::deque<std::string>& tree)
{
  std::string half_empty_string = std::string(WIDTH / 2 - 10, ' ');

  for (auto& row : tree) {
    row = half_empty_string + row + half_empty_string;
  }

  size_t empty_height = HEIGHT - tree.size();
  std::string empty_string = std::string(WIDTH, ' ');
  for (size_t i = 0; i < empty_height; ++i) {
    tree.push_front(empty_string);
  }
}

void make_frame(std::vector<std::string>& frame, 
  const std::vector<std::string>& snow, const std::deque<std::string>& tree)
{
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (snow[i][j] == '*') {
        frame[i][j] = '*';
      }
      else if (tree[i][j] != ' ') {
        frame[i][j] = tree[i][j];
      }
      else {
        frame[i][j] = ' ';
      }
    }
  }
}

void draw_frame(const std::vector<std::string>& frame)
{
  for (const auto& row : frame) {
    std::cout << row << '\n';
  }
  std::cout << std::setw(WIDTH + 1) << std::setfill('*') << '\n';
}

void move_snow(std::vector<std::string>& snow)
{
  auto rbegin = snow.rbegin();
  auto prerend = std::prev(snow.rend());
  for (auto it = rbegin; it != prerend; ++it) {
    *it = std::move(*std::next(it));
  }
  snow[0] = make_snow_row();
}

void sleep(const int64_t ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void clear_console()
{
  system("cls");
}


int main() 
{
  std::deque<std::string> tree = {
        "               /\\               ",
        "              / \\\\             ",
        "             /   \\\\            ",
        "            /   o \\\\           ",
        "           /_ $   _\\\\          ",
        "            /     \\\\           ",
        "           /    $  \\\\          ",
        "          / $     o \\\\         ",
        "         /_         _\\\\        ",
        "          /    o    \\\\         ",
        "         /  o        \\\\        ",
        "        /o       o    \\\\       ",
        "       /_     $       _\\\\      ",
        "        /       o     \\\\       ",
        "       /      o        \\\\      ",
        "      /   $        o    \\\\     ",
        "     /_        $      o _\\\\    ",
        "      /     o     o     \\\\     ",
        "     /    o     $        \\\\    ",
        "    / $        o     o    \\\\   ",
        "   /_______________________\\\\  ",
        "             |    |              ",
        "             |    |              "
  };
  init_tree(tree);

  std::vector<std::string> snow(HEIGHT);
  init_snow(snow);

  std::vector<std::string> frame(HEIGHT, std::string(WIDTH, ' '));
  while (true) {
    make_frame(frame, snow, tree);
    draw_frame(frame);
    move_snow(snow);

    sleep(SLEEP_PER_FRAME);
    clear_console();
  }

  return 0;
}
