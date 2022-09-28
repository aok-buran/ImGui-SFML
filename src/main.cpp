#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <fstream>

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 800;

const char OUTPUT_PATH[255] = "D:/Programming/Files/out.txt";
const char INPUT_PATH[255] = "D:/Programming/Files/in.txt";

// первое множество
static const int SET_1 = 0;
// второе множество
static const int SET_2 = 1;
// пересечение множеств
static const int SET_CROSSED = 2;
// разность множеств
static const int SET_SINGLE = 3;

struct Point {
    sf::Vector2<int> pos;
    int setNum;

    Point(const sf::Vector2<int> &pos, int setNum) : pos(pos), setNum(setNum) {
    }

    static Point randomPoint() {
        return Point(sf::Vector2<int>(
                             abs(rand() % WINDOW_SIZE_X),
                             abs(rand() % WINDOW_SIZE_Y)),
                     abs(rand() % 2)
        );
    }
};


sf::Color bgColor;
float color[3] = {0.12f, 0.12f, 0.13f};


std::vector<Point> points;

std::vector<Point> resPoints;

int lastAddPosBuf[2] = {0, 0};

int lastRandomizeBuf[1] = {10};

void randomize(int cnt) {
    for (int i = 0; i < cnt; i++) {
        points.emplace_back(Point::randomPoint());
    }
}

void solve() {
    resPoints.clear();
    for (int i = 0; i < points.size(); i++)
        for (int j = i + 1; j < points.size(); j++)
            if (points[i].pos == points[j].pos)
                resPoints.emplace_back(points[i]);

}

// рисование задачи на невидимом окне во всё окно приложения
void RenderTask() {
    // задаём левый верхний край невидимого окна
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    // задаём правый нижний край невидимого окна
    ImGui::SetNextWindowSize(ImVec2(WINDOW_SIZE_X, WINDOW_SIZE_Y));

    ImGui::Begin("Overlay", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

    auto pDrawList = ImGui::GetWindowDrawList();

    for (auto point: points) {
        pDrawList->AddCircleFilled(
                sf::Vector2<int>(point.pos.x, point.pos.y),
                3,
                point.setNum == 0 ? ImColor(200, 100, 150) : ImColor(100, 200, 150),
                20
        );
    }

    for (auto point: resPoints) {
        pDrawList->AddCircleFilled(
                sf::Vector2<int>(point.pos.x, point.pos.y),
                3,
                ImColor(100, 150, 200),
                20
        );
    }


//        pDrawList->AddRect(ImVec2(10, 10), ImVec2(100, 100), ImColor(255, 0, 0));
//        pDrawList->AddText(ImVec2(10, 10), ImColor(255, 0, 0), "test");

    ImGui::End();
}

void ShowBackgroundSetting() {
    if (!ImGui::CollapsingHeader("Background"))
        return;

    // Инструмент выбора цвета
    if (ImGui::ColorEdit3("Background color", color)) {
        // код вызывается при изменении значения, поэтому всё
        // обновляется автоматически
        bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
        bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
        bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
    }
}

void ShowAddElem() {
    if (!ImGui::CollapsingHeader("Add Elem"))
        return;


    // Инструмент выбора цвета
    if (ImGui::DragInt2("Coords", lastAddPosBuf)) {

    }

    ImGui::PushID(0);
    if (ImGui::Button("Set 1"))
        points.emplace_back(Point(sf::Vector2<int>(lastAddPosBuf[0], lastAddPosBuf[1]), SET_1));

    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(1);
    if (ImGui::Button("Set 2"))
        points.emplace_back(Point(sf::Vector2<int>(lastAddPosBuf[0], lastAddPosBuf[1]), SET_2));
    ImGui::PopID();
}

void ShowRandomize() {
    if (!ImGui::CollapsingHeader("Randomize"))
        return;

    ImGui::PushID(0);

    // Инструмент выбора цвета
    if (ImGui::DragInt("Count", lastRandomizeBuf, 0.1, 0, 100)) {

    }

    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(1);
    if (ImGui::Button("Add"))
        randomize(lastRandomizeBuf[0]);
    ImGui::PopID();
}


void ShowFiles() {
    if (!ImGui::CollapsingHeader("Files"))
        return;

    ImGui::PushID(0);
    if (ImGui::Button("Load")) {
        std::ifstream input(INPUT_PATH);
        points.clear();
        resPoints.clear();
        while (!input.eof()) {
            int x, y, s;
            input >> x;
            input >> y;
            input >> s;
            points.emplace_back(Point(sf::Vector2<int>(x, y), s));
        }
    }
    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(1);
    if (ImGui::Button("Save")) {

        std::ofstream output;
        output.open(OUTPUT_PATH);
        std::cout << OUTPUT_PATH << std::endl;

        for (auto point: points) {
            std::cout << "point" << std::endl;
            output << point.pos.x << " " << point.pos.y << " " << point.setNum << std::endl;
        }


        output.close();
        points.emplace_back(Point(sf::Vector2<int>(lastAddPosBuf[0], lastAddPosBuf[1]), 1));
    }
    ImGui::PopID();

}

void ShowSolve() {
    if (!ImGui::CollapsingHeader("Solve"))
        return;
    ImGui::PushID(0);
    if (ImGui::Button("Solve")) {
        solve();
    }

    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID(1);

    if (ImGui::Button("Clear")) {
        points.clear();
        resPoints.clear();
    }
    ImGui::PopID();
}

void ShowHelp() {
    if (!ImGui::CollapsingHeader("Help"))
        return;

    ImGui::Text("ABOUT THIS DEMO:");
    ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
    ImGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
    ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
                      "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");
    ImGui::Separator();

    ImGui::Text("TASK:");
    ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
    ImGui::BulletText("See comments in imgui.cpp.");
    ImGui::BulletText("See example applications in the examples/ folder.");
    ImGui::BulletText("Read the FAQ at http://www.dearimgui.org/faq/");
    ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
    ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
    ImGui::Separator();

}


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);


    bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);

    // здесь мы будем использовать массив char. Чтобы использовать
    // std::string нужно сделать действия, описанные во второй части
    char windowTitle[255] = "ImGui + SFML = <3";
    window.setTitle(windowTitle);


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (!ImGui::GetIO().WantCaptureMouse && !(event.mouseButton.x == 0 && event.mouseButton.y == 0)) {
                    lastAddPosBuf[0] = event.mouseButton.x;
                    lastAddPosBuf[1] = event.mouseButton.y;
                    if (event.mouseButton.button == sf::Mouse::Button::Left)
                        points.emplace_back(sf::Vector2<int>(event.mouseButton.x, event.mouseButton.y), SET_1);
                    else
                        points.emplace_back(sf::Vector2<int>(event.mouseButton.x, event.mouseButton.y), SET_2);
                }
            }

        }

        ImGui::SFML::Update(window, deltaClock.restart());

        RenderTask();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.13f, 0.8f)); // Set window background to red

        ImGui::Begin("Control"); // создаём окно

        ShowBackgroundSetting();
        ShowAddElem();
        ShowRandomize();
        ShowFiles();
        ShowSolve();
        ShowHelp();
        ImGui::End(); // end window


        // ImGui::ShowDemoWindow();


        ImGui::PopStyleColor();

        window.clear(bgColor); // заполняем окно заданным цветом
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}