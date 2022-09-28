#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

// первое множество
static const int SET_1 = 0;
// второе множество
static const int SET_2 = 1;
// пересечение множеств
static const int SET_CROSSED = 2;
// разность множеств
static const int SET_SINGLE = 3;

// Ширина окна
static const int WINDOW_SIZE_X = 800;
// Высота окна
static const int WINDOW_SIZE_Y = 800;

// Точка
struct Point {
    // положение
    sf::Vector2<int> pos;
    // номер множества
    int setNum;

    // конструктор
    Point(const sf::Vector2<int> &pos, int setNum) : pos(pos), setNum(setNum) {
    }
};

// динамический список точек
std::vector<Point> points;

// цвет фона
static sf::Color bgColor;
// значение цвета по умолчанию
float color[3] = {0.12f, 0.12f, 0.13f};

// буфер, хранящий координаты последней добавленной вершины
int lastAddPosBuf[2] = {0, 0};

// задать цвет фона по вещественному массиву компонент
static void setColor(float *pDouble) {
    bgColor.r = static_cast<sf::Uint8>(pDouble[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(pDouble[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(pDouble[2] * 255.f);
}

// рисование параметров цвета
void ShowBackgroundSetting() {
    // если не раскрыта панель `Background`
    if (!ImGui::CollapsingHeader("Background"))
        // заканчиваем выполнение
        return;

    // Инструмент выбора цвета
    if (ImGui::ColorEdit3("Background color", color)) {
        // код вызывается при изменении значения
        // задаём цвет фона
        setColor(color);
    }
}

// рисование задачи на невидимом окне во всё окно приложения
void RenderTask() {
    // задаём левый верхний край невидимого окна
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    // задаём правый нижний край невидимого окна
    ImGui::SetNextWindowSize(ImVec2(WINDOW_SIZE_X, WINDOW_SIZE_Y));
    // создаём невидимое окно
    ImGui::Begin("Overlay", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    // получаем список примитивов, которые будут нарисованы
    auto pDrawList = ImGui::GetWindowDrawList();

    // перебираем точки из динамического массива точек
    for (auto point: points) {
        // добавляем в список рисования круг
        pDrawList->AddCircleFilled(
                sf::Vector2<int>(point.pos.x, point.pos.y),
                3,
                point.setNum == 0 ? ImColor(200, 100, 150) : ImColor(100, 200, 150),
                20
        );
    }
    // заканчиваем рисование окна
    ImGui::End();
}

// ручное добавление элементов
void ShowAddElem() {
    // если не раскрыта панель `Add Elem`
    if (!ImGui::CollapsingHeader("Add Elem"))
        // заканчиваем выполнение
        return;


    // Инструмент выбора цвета
    if (ImGui::DragInt2("Coords", lastAddPosBuf, 0.5f, 0, std::min(WINDOW_SIZE_X, WINDOW_SIZE_Y))) {
        // никаких действий не требуется, достаточно
        // тех изменений буфера, которые imGui выполняет
        // автоматически
    }

    // фиксируем id равный 0 для первого элемента
    ImGui::PushID(0);
    // если нажата кнопка `Set 1`
    if (ImGui::Button("Set 1"))
        // добавляем то добавляем в список точку, принадлежащую первому множеству
        points.emplace_back(Point(sf::Vector2<int>(lastAddPosBuf[0], lastAddPosBuf[1]), SET_1));
    // восстанавливаем буфер id
    ImGui::PopID();

    // говорим imGui, что следующий элемент нужно рисовать на той же линии
    ImGui::SameLine();
    // задаём id, равный одному
    ImGui::PushID(1);
    // если нажата кнопка `Set 2`
    if (ImGui::Button("Set 2"))
        // добавляем то добавляем в список точку, принадлежащую второму множеству
        points.emplace_back(Point(sf::Vector2<int>(lastAddPosBuf[0], lastAddPosBuf[1]), SET_2));
    // восстанавливаем буфер id
    ImGui::PopID();
}

// главный метод
int main() {
    // создаём окно для рисования
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Geometry Project 10");
    // задаём частоту перерисовки окна
    window.setFramerateLimit(60);
    // инициализация imgui+sfml
    ImGui::SFML::Init(window);

    // задаём цвет фона
    setColor(color);

    // переменная таймера
    sf::Clock deltaClock;
    // пока окно открыто, запускаем бесконечный цикл
    while (window.isOpen()) {
        // создаём событие sfml
        sf::Event event;
        // пока окно принимает события
        while (window.pollEvent(event)) {
            // отправляем события на обработку sfml
            ImGui::SFML::ProcessEvent(event);

            // если событие - это закрытие окна
            if (event.type == sf::Event::Closed) {
                // закрываем окно
                window.close();
            }
            // если событие - это клик мышью
            if (event.type == sf::Event::MouseButtonPressed) {
                // если мышь не обрабатывается элементами imGui
                if (!ImGui::GetIO().WantCaptureMouse) {
                    // меняем координаты последней добавленной точки
                    lastAddPosBuf[0] = event.mouseButton.x;
                    lastAddPosBuf[1] = event.mouseButton.y;
                    // если левая кнопка мыши
                    if (event.mouseButton.button == sf::Mouse::Button::Left)
                        points.emplace_back(sf::Vector2<int>(event.mouseButton.x, event.mouseButton.y), SET_1);
                    else
                        points.emplace_back(sf::Vector2<int>(event.mouseButton.x, event.mouseButton.y), SET_2);
                }
            }
        }

        // запускаем обновление окна по таймеру с заданной частотой
        ImGui::SFML::Update(window, deltaClock.restart());

        // рисование задания
        RenderTask();

        // делаем окно полупрозрачным
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.13f, 0.8f));

        // создаём окно управления
        ImGui::Begin("Control");

        // рисование параметров цвета
        ShowBackgroundSetting();
        // ручное добавление элементов
        ShowAddElem();

        // конец рисования окна
        ImGui::End();

        // Возвращаем цвет окна к исходному
        ImGui::PopStyleColor();

        // очищаем окно
        window.clear(bgColor);
        // рисуем по окну средствами imgui+sfml
        ImGui::SFML::Render(window);
        // отображаем изменения на окне
        window.display();
    }

    // завершаем работу imgui+sfml
    ImGui::SFML::Shutdown();

    return 0;
}