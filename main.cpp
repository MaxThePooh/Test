#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//#include <string>
//#include <fstream>
#include <iostream>
//#include <cstdio>  /* defines FILENAME_MAX */
//#include <filesystem>

#define SFML_STATIC

#ifdef MACOS_BUNDLE
#include <CoreFoundation/CoreFoundation.h>
void Path() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *) path, PATH_MAX))
        exit(11);

    CFRelease(resourcesURL);

    chdir(path);
}
#endif



int calculatesqrt(long int num,int target)
{
    int sqrt=0;
    while(num>=target)
    {
        num/=target;
        sqrt++;
    }
    return sqrt;
}


struct FONT{
    sf::Font font;
    FONT()
    {
#ifdef MACOS_BUNDLE
        Path();
#endif
        font.loadFromFile("./Sources/font.otf");
    }
};

FONT Font;

const unsigned int width=900;
const unsigned int height=600;

const double startspeed=-(float)width/1200;


class Cube
{
private:
    sf::RectangleShape cube;
    sf::Text massText;
    long int mass;
    long double speed;
public:
    Cube()
    {
        mass=1;
        cube.setSize(sf::Vector2f(5,5));
        cube.setFillColor(sf::Color(138, 138, 138));
        massText.setFillColor(sf::Color::White);
        massText.setCharacterSize(75);
        massText.setFont(Font.font);
        speed=0;
    }
    sf::RectangleShape get_cube()
    {
        return cube;
    }
    sf::FloatRect get_cube_bounds()
    {
        return cube.getGlobalBounds();
    }
    void set_speed(long double targetspeed)
    {
        speed=targetspeed;
    }
    void set_mass(long int targetmass,sf::RenderWindow &targetWindow)
    {
        sf::Vector2u targetWindowSize=targetWindow.getSize();

        mass=targetmass;
        speed=0;

        massText.setString(std::to_wstring(mass)+L"kg");
        massText.setOrigin(massText.getGlobalBounds().width/2.f,massText.getGlobalBounds().height/2.f);

        float sideSize=targetWindowSize.y*0.1+targetWindowSize.y*0.05*(float)targetmass>targetWindowSize.y*0.4
                ?targetWindowSize.y*0.4:targetWindowSize.y*0.1f+targetWindowSize.y*0.05*targetmass;
        cube.setSize(sf::Vector2f(sideSize,sideSize));
    }
    void set_position(sf::Vector2f pos)
    {
        cube.setPosition(pos);
    }
    long double get_speed()
    {
        return speed;
    }
    long double get_mass()
    {
        return mass;
    }
    void update()
    {
        cube.move((float)speed,0);
    }
    void correctForWindow(sf::RenderWindow &window)
    {
        massText.setCharacterSize(window.getSize().y/23.3f);
        massText.setOrigin(massText.getGlobalBounds().width/2,massText.getGlobalBounds().height/2);
        sf::Vector2u targetWindowSize=window.getSize();

        float sideSize=targetWindowSize.y*0.1f+targetWindowSize.y*0.05f*(float)mass>targetWindowSize.y*0.4f
                       ?targetWindowSize.y*0.4f:targetWindowSize.y*0.1f+targetWindowSize.y*0.05f*mass;
        cube.setSize(sf::Vector2f(sideSize,sideSize));
    }
    void draw(sf::RenderWindow &window)
    {

        window.draw(cube);

        massText.setPosition(sf::Vector2f (cube.getPosition().x+cube.getSize().x/2.f,
                                           cube.getPosition().y-(float)massText.getCharacterSize()-5.f));
        window.draw(massText);
    }
};

class Walls
{
private:
    sf::RectangleShape wall;
    sf::RectangleShape roof;
public:
    Walls()=default;



    Walls(sf::RenderWindow &target_window)
    {
        sf::Vector2<unsigned int> window_size(target_window.getSize());

        sf::Vector2f wall_size(window_size.x/100.f,window_size.y*0.75);
        wall.setSize(wall_size);

        sf::Vector2f roof_size(window_size.x/100.f, window_size.x*0.875);
        roof.setSize(roof_size);

        wall.setFillColor(sf::Color::White);
        roof.setFillColor(sf::Color::White);

        wall.setOrigin(wall_size/2.f);
        roof.setOrigin(roof_size/2.f);

        roof.setRotation(90);
    }
    sf::RectangleShape get_wall()
    {
        return wall;
    }
    sf::RectangleShape get_roof()
    {
        return roof;
    }
    void set_wall_pos(float x,float y)
    {
        wall.setPosition(x,y);
    }
    void set_roof_pos(float x,float y)
    {
        roof.setPosition(x,y);
    }
};

class mainProccesion {
private:
    sf::SoundBuffer soundbuffer;
    sf::Sound sound;
    Walls walls;
    Cube cubeA;
    Cube cubeB;
    sf::Text numCollisions;
    sf::Int32 milsec;
    long int collisions;
public:
    mainProccesion()=default;
    mainProccesion(sf::RenderWindow &targetWindow) {
        collisions = 0;
        soundbuffer.loadFromFile("./Sources/knock.ogg");
        sound.setBuffer(soundbuffer);
        walls = Walls(targetWindow);
        walls.set_wall_pos(targetWindow.getSize().x * 0.125f + walls.get_wall().getSize().x / 2,
                           targetWindow.getSize().y * 0.25f/*+walls.get_wall().getSize().y*0.5f*/);
        walls.set_roof_pos(targetWindow.getSize().x * 0.125f + walls.get_roof().getSize().y / 2,
                           targetWindow.getSize().y * 0.625f);

        numCollisions.setFillColor(sf::Color::White);
        numCollisions.setCharacterSize(75);
        numCollisions.setFont(Font.font);
        numCollisions.setPosition(targetWindow.getSize().x*0.6,targetWindow.getSize().y*0.1);
    }
    void correctForwindow(sf::RenderWindow &targetWindow)
    {
        cubeB.correctForWindow(targetWindow);
        cubeA.correctForWindow(targetWindow);

        milsec=0;
        collisions = 0;
        soundbuffer.loadFromFile("./Sources/knock.ogg");
        sound.setBuffer(soundbuffer);
        walls = Walls(targetWindow);
        walls.set_wall_pos(targetWindow.getSize().x * 0.125f + walls.get_wall().getSize().x / 2,
                           targetWindow.getSize().y * 0.25f/*+walls.get_wall().getSize().y*0.5f*/);
        walls.set_roof_pos(targetWindow.getSize().x * 0.125f + walls.get_roof().getSize().y / 2,
                           targetWindow.getSize().y * 0.625f);

        numCollisions.setFillColor(sf::Color::White);
        numCollisions.setCharacterSize(targetWindow.getSize().y/21.3f);
        numCollisions.setFont(Font.font);
        numCollisions.setPosition(targetWindow.getSize().x*0.6,targetWindow.getSize().y*0.1);
    }
    void set_cubes_mass(long int massA, long int massB, sf::RenderWindow &targetwindow) {
        cubeA.set_mass(massA, targetwindow);
        cubeB.set_mass(massB, targetwindow);
        cubeA.set_position(sf::Vector2f(walls.get_wall().getPosition().x + targetwindow.getSize().x / 5,
                                        walls.get_roof().getPosition().y + walls.get_roof().getSize().x / 2 -
                                        cubeA.get_cube().getSize().y-walls.get_roof().getSize().x));
        cubeB.set_position(sf::Vector2f(walls.get_wall().getPosition().x + targetwindow.getSize().x / 1.5,
                                        walls.get_roof().getPosition().y + walls.get_roof().getSize().x / 2 -
                                        cubeB.get_cube().getSize().y-walls.get_roof().getSize().x));
        cubeB.set_speed(startspeed);
        collisions=0;
    }

    void draw(sf::RenderWindow &targetWindow) {

        targetWindow.draw(walls.get_wall());
        targetWindow.draw(walls.get_roof());
        cubeA.draw(targetWindow);
        cubeB.draw(targetWindow);

        numCollisions.setString(L"#Collisions "+std::to_wstring(collisions));
        targetWindow.draw(numCollisions);
    }
    void update(sf::Int32 elapsedtime) {
        milsec+=elapsedtime;
        int halfXcubeA=cubeA.get_cube().getSize().x/2;
        int halfXsizewallANDroof=walls.get_wall().getSize().x/2;
        sf::Vector2f poscubeA(cubeA.get_cube().getPosition());
        sf::Vector2f poscubeB(cubeB.get_cube().getPosition());
        sf::Vector2f poswall(walls.get_wall().getPosition());
        do {
            poscubeB=cubeB.get_cube().getPosition();
            poscubeA=cubeA.get_cube().getPosition();
            if (poscubeA.x + halfXcubeA * 2 >= poscubeB.x&&cubeA.get_speed()>=0) {
                long double Va = cubeA.get_speed();
                long double Vb = cubeB.get_speed();
                long int Ma = cubeA.get_mass();
                long int Mb = cubeB.get_mass();
                cubeA.set_speed((Mb * (Vb - Va) + Ma * Va + Mb * Vb) / (Ma + Mb));
                cubeB.set_speed((Ma * (Va - Vb) + Ma * Va + Mb * Vb) / (Ma + Mb));
                //ubeA.set_position(sf::Vector2f(poscubeB.x - halfXcubeA * 2, poscubeA.y));
                if (milsec > 500) {
                    sound.play();
                    milsec = 0;
                }
                collisions++;
            }
            if (poscubeA.x <= poswall.x + halfXsizewallANDroof && cubeA.get_speed()<0) {
                cubeA.set_speed(-cubeA.get_speed());
                //cubeA.set_position(sf::Vector2f(poswall.x + halfXsizewallANDroof - 1, poscubeA.y));
                if (milsec > 500) {
                    sound.play();
                    milsec = 0;
                }
                collisions++;
            }
            cubeA.update();
            cubeB.update();
        }while(poscubeB.x-poswall.x-halfXsizewallANDroof<=halfXcubeA*2);


}
};


class Menu
{
private:
    sf::Text inBox;
    sf::Text WHAT;
    sf::Text WarningText;
    sf::Image WarningImage;
    sf::Texture WarningTexture;
    sf::Sprite WarningSign;
    sf::RectangleShape Box;
    long int mass;
    bool end;
public:
    Menu()
    {
        end=0;
        mass=1L;

        inBox.setFillColor(sf::Color::White);
        inBox.setOrigin(inBox.getGlobalBounds().width/2,inBox.getGlobalBounds().height/2);
        inBox.setCharacterSize(75);
        inBox.setFont(Font.font);

        WarningText.setFillColor(sf::Color::Red);
        WarningText.setCharacterSize(85);
        WarningText.setFont(Font.font);
        WarningText.setString(L"Lag Warning\ntoo big mass");
        WarningText.setOrigin(WarningText.getGlobalBounds().width/2,WarningText.getGlobalBounds().height/2);


        WHAT.setFillColor(sf::Color::White);
        WHAT.setCharacterSize(60);
        WHAT.setFont(Font.font);
        WHAT.setString(L"Enter-start\nR-restart");
        WHAT.setOrigin(WHAT.getGlobalBounds().width/2,WHAT.getGlobalBounds().height/2);



        WarningImage.loadFromFile("./Sources/WarningSign.png");
       WarningImage.createMaskFromColor(sf::Color::Transparent);
        WarningTexture.loadFromImage(WarningImage);
        WarningSign.setTexture(WarningTexture);
        WarningSign.setScale(0.25f,0.25f);

        Box.setSize(sf::Vector2f(1920,96));
        Box.setOutlineThickness(8);
        Box.setOutlineColor(sf::Color::White);
        Box.setFillColor(sf::Color(0,0,0,255));
        Box.setOrigin(Box.getGlobalBounds().width/2,Box.getGlobalBounds().height/2);
    }
    void correctForWindow(sf::RenderWindow &window)
    {
        sf::Vector2u WindowSize=window.getSize();



        Box.setSize(sf::Vector2f (WindowSize.x*0.8f,WindowSize.y*0.06f));
        Box.setOrigin(Box.getGlobalBounds().width/2.f,Box.getGlobalBounds().height/2.f);
        Box.setOutlineThickness(Box.getSize().y/10.f<1.f?1.f:Box.getSize().y/10.f);
        Box.setPosition(WindowSize.x*0.5f,WindowSize.y*0.2f);

        inBox.setCharacterSize(WindowSize.y/23.3f);
        inBox.setPosition(Box.getPosition().x-Box.getSize().x/2.f, Box.getPosition().y);

        WHAT.setCharacterSize(WindowSize.y/26.6);
        WHAT.setPosition(Box.getPosition().x, Box.getPosition().y+Box.getSize().y*0.5+WHAT.getCharacterSize());
        WHAT.setOrigin(WHAT.getGlobalBounds().width/2,WHAT.getGlobalBounds().height/2);


        WarningText.setPosition(WindowSize.x*0.5,WindowSize.y*0.8);
        WarningText.setCharacterSize(WindowSize.y/18.8);
        WarningText.setOrigin(WarningText.getGlobalBounds().width/2,WarningText.getGlobalBounds().height/2);

        WarningSign.setScale((float)(WindowSize.x/2400.f*0.25f),(float)(WindowSize.y/1600.f*0.25f));
        WarningSign.setPosition(WarningText.getPosition().x-WarningSign.getGlobalBounds().width/2,WarningText.getPosition().y
                                -WarningText.getCharacterSize()-WarningSign.getGlobalBounds().height);

    }
    void draw(sf::RenderWindow &window)
    {
        window.draw(Box);
        inBox.setString(std::to_wstring(mass)+L"(100^"+std::to_wstring(calculatesqrt(mass,100L))+L")kg");
        inBox.setOrigin(0,inBox.getGlobalBounds().height/2.f);
        window.draw(inBox);
        window.draw(WHAT);
        if(mass>=1000000000000) {
            window.draw(WarningText);
            window.draw(WarningSign);
        }
    }
    bool checkInput(sf::Event &event,long int &targetmass)
    {
            if(event.text.unicode)
            {
                wchar_t text=(wchar_t )event.text.unicode;
                if(text==8)
                    mass/=10;
                if(text==10 || text==13)//ENTER
                {
                    targetmass=mass;
                    return 1;
                }
                if(text>=48 && text<=57)
                {
                    std::wstring trash(1,text);
                    mass=mass*10L+std::stol(trash, nullptr);
                }

                return 0;
            }
    }
};

class App
{
private:
    sf::RenderWindow *window;
    sf::Clock clock;
    sf::Time time;
    sf::Event event;
    unsigned int current;
    long int MassVolume;
    mainProccesion proccesion;
    Menu menu;
public:
    App()
    {
        current=0;
        MassVolume=1L;
        window=new sf::RenderWindow(sf::VideoMode(width,height),"The best Pi calculator");//,sf::Style::Fullscreen);
        window->setFramerateLimit(120);

        proccesion.set_cubes_mass(1,1,*window);
        menu.correctForWindow(*window);
        proccesion.correctForwindow(*window);
    }
    ~App()
    {
        delete window;
    }
    void While()
    {
        while (window->isOpen())
        {
            time+=clock.getElapsedTime();
            while (window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window->close();
                if(event.type==sf::Event::KeyPressed)
                    if(event.key.code==sf::Keyboard::Q)
                        window->close();
                if(event.type==sf::Event::TextEntered)
                {
                    switch (current) {
                        case 0U:
                            current=menu.checkInput(event,MassVolume);
                            if(current!=0)
                                proccesion.set_cubes_mass(1,MassVolume,*window);
                            break;
                        case 1U:
                            if(event.text.unicode<128)
                            {
                                wchar_t input=(wchar_t )event.text.unicode;
                                if(input==1082  || input==1050 || input==114|| input==82)
                                {
                                    current=0;
                                }

                            }
                            break;
                    }
                    if(event.type==sf::Event::Resized)
                    {
                        window->setSize(sf::Vector2u(event.size.width,event.size.height));
                        proccesion.correctForwindow(*window);
                        menu.correctForWindow(*window);
                    }
                }

            }


            switch (current) {
                case 0:
                    RunMenu();
                    break;
                case 1:
                    RunProccesion();
                    break;
            }
            clock.restart();
        }

    }
    void RunProccesion()
    {
        proccesion.update(time.asMilliseconds());
        window->clear();
        proccesion.draw(*window);
        window->display();
    }
    void RunMenu()
    {
        window->clear();
        menu.draw(*window);
        window->display();
    }
};


void BresenhamCircle(sf::VertexArray &array, int _x, int _y, int radius) {
    int x = 0, y = radius, gap = 0, delta = (2 - 2 * radius);
    while (y >= 0) {
        sf::Vertex trash;
        trash.color=sf::Color::Red;
        trash.position = sf::Vector2f(_x + x, _y + y-radius*0.5);//4
        array.append(trash);
        trash.position = sf::Vector2f(_x + x, _y - y-radius*0.5);//2
        array.append(trash);
        trash.position = sf::Vector2f(_x - x, _y - y-radius*0.5);//1
        array.append(trash);
        trash.position = sf::Vector2f(_x - x, _y + y-radius*0.5);//3
        array.append(trash);
        gap = 2 * (delta + y) - 1;
        if (delta < 0 && gap <= 0) {
            x++;
            delta += 2 * x + 1;
            continue;
        }
        if (delta > 0 && gap > 0) {
            y--;
            delta -= 2 * y + 1;
            continue;
        }
        x++;
        delta += 2 * (x - y);
        y--;
    }
}


    int main(int args,char** argv) {






App app;
app.While();
    return 0;
}
