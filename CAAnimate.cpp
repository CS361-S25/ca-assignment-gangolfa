#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate
{

    // grid width and height
    const int num_h_boxes = 10;
    const int num_w_boxes = 15;
    const double RECT_SIDE = 25;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    // My variables:
    int x = 0;
    int y = 0;

    // some vectors to hold information about the CA
    std::vector<std::vector<int>> cells;
    std::vector<std::vector<int>> cells_dead;

    // where we'll draw
    emp::web::Canvas canvas{width, height, "canvas"};

public:
    CAAnimator()
    {
        // shove canvas into the div
        // along with some control buttons
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // fill the vectors with 0 to start
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        // showing how to set a cell to 'alive'
        cells[1][0] = 1;
        cells[2][1] = 1;
        cells[0][2] = 1;
        cells[1][2] = 1;
        cells[2][2] = 1;
    }

    void DoFrame() override
    {
        canvas.Clear();

        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {

                if (cells[x][y] == 0)
                {
                    // Draw a rectangle on the canvas filled with white and outlined in black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                else
                {
                    // Draw a rectangle filled with black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "black", "black");
                }
            }
        }

        // My code:
        cells_dead = cells;
        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {
                // Check 8 neighbors. You want to check no matter dead or alive, then set to dead or alive
                int neighbors = 0;
                // Right
                if (x + 1 < num_w_boxes && cells[x + 1][y] == 1)
                {
                    neighbors++;
                }
                // Left
                if (x - 1 >= 0 && cells[x - 1][y] == 1)
                {
                    neighbors++;
                }
                // Up
                if (y + 1 < num_h_boxes && cells[x][y + 1] == 1)
                {
                    neighbors++;
                }
                // Down
                if (y - 1 >= 0 && cells[x][y - 1] == 1)
                {
                    neighbors++;
                }
                // Up-Right
                if (x + 1 < num_w_boxes && y + 1 < num_h_boxes && cells[x + 1][y + 1] == 1)
                {
                    neighbors++;
                }
                // Up-Left
                if (x - 1 >= 0 && y + 1 < num_h_boxes && cells[x - 1][y + 1] == 1)
                {
                    neighbors++;
                }
                // Down-Right
                if (x + 1 < num_w_boxes && y - 1 >= 0 && cells[x + 1][y - 1] == 1)
                {
                    neighbors++;
                }
                // Down-Left
                if (x - 1 >= 0 && y - 1 >= 0 && cells[x - 1][y - 1] == 1)
                {
                    neighbors++;
                }
                // Now check the rules
                if (cells[x][y] == 1)
                {
                    if (neighbors < 2 || neighbors > 3)
                    {
                        cells_dead[x][y] = 0; // Dies
                    }
                    else
                    {
                        cells_dead[x][y] = 1; // Survives
                    }
                }
                else
                {
                    if (neighbors == 3)
                    {
                        cells_dead[x][y] = 1; // Becomes alive
                    }
                    else
                    {
                        cells_dead[x][y] = 0; // Stays dead
                    }
                }
            }
        }
        cells = cells_dead;
    }

    // Rule #1
}

// Place to update the values of all the cells based on CA rules

;

CAAnimator animator;

int main()
{
    // Have animator call DoFrame once to start
    animator.Step();
}