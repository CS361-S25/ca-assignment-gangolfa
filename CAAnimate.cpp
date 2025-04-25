#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate
{

    // Grid Layout
    const int num_h_boxes = 10;                   // Rows
    const int num_w_boxes = 15;                   // Columns
    const double RECT_SIDE = 25;                  // Square Size
    const double width{num_w_boxes * RECT_SIDE};  // Canvas Width
    const double height{num_h_boxes * RECT_SIDE}; // Canvas Height

    // My variables:
    int x = 0;
    int y = 0;

    // Grid of Cells: 0 is dead, 1 is alive
    std::vector<std::vector<int>> cells;
    std::vector<std::vector<int>> cells_dead;

    // Canvas
    emp::web::Canvas canvas{width, height, "canvas"};

public:
    // Constructor
    CAAnimator()
    {
        // Add Canvas and Control buttons to the webpage
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // Fill grid with all dead cells
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        // Manually Set up Glider Pattern
        cells[1][0] = 1;
        cells[2][1] = 1;
        cells[0][2] = 1;
        cells[1][2] = 1;
        cells[2][2] = 1;
    }

    void DoFrame() override
    {
        canvas.Clear();

        // Draw The Grid
        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {

                if (cells[x][y] == 0)
                {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                else
                {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "black", "black");
                }
            }
        }

        // Conway's Game of Life
        cells_dead = cells; // Copy the current state of cells

        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {
                int neighbors = 0;

                // Count live neighbors for toroidal shape
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        if (dx == 0 && dy == 0)
                            continue; // Skip the current cell

                        // Compute neighbor coordinates to wrap
                        int nx = (x + dx + num_w_boxes) % num_w_boxes;
                        int ny = (y + dy + num_h_boxes) % num_h_boxes;

                        if (cells[nx][ny] == 1)
                        {
                            neighbors++;
                        }
                    }
                }

                // Game of Life Rules
                if (cells[x][y] == 1)
                {
                    // Dies from underpopulation or overpopulation
                    if (neighbors < 2 || neighbors > 3)
                    {
                        cells_dead[x][y] = 0; // Dies
                    }
                    else
                    {
                        cells_dead[x][y] = 1; // Survives
                    }
                }
                else // Current cell is dead
                {
                    // Lives through reproduction
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

        cells = cells_dead; // Update grid
    }
};

CAAnimator animator;

int main()
{
    // Have animator call DoFrame once to start
    animator.Step();
}