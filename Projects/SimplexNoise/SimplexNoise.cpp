//
// Created by Max Roschupkin on 18.03.2021.
//



#include "SFML/Graphics.hpp"
#include "EventHandler.hpp"
#include "SimplexNoise.h"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include <cstdarg>
#include <exception>


template<typename T>
class ThreadCrutch{
public:
    ThreadCrutch()
    : exit(false)
    {}

    template<class C>
    void setF(T (C::*func)(),C* c)
    {
        join();
        std::packaged_task<T()> packagedTask(std::bind(func,c));
        future=packagedTask.get_future();
        voidF=std::thread(std::move(packagedTask));
    }
    void setF(T (*func)())
    {
        if(ready())
            future.get();
        if(running())
            terminate();
        std::packaged_task<T()> packagedTask(func);
        future=packagedTask.get_future();
        voidF=std::thread(std::move(packagedTask));
    }
    bool running() const{
        return future.valid()?future.wait_for(std::chrono::milliseconds(0))==std::future_status::timeout:false;
    }
    bool ready() const{
        return future.valid()?future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready:false;
    }
    bool exist() const{
        return running() || ready();
    }
    bool getExit() const{
        return exit;
    }
    void terminate(){
        exit=true;
        join();
    }
    void join(){
        if(voidF.joinable())
            voidF.join();
        exit=false;
    }
    ~ThreadCrutch(){
        terminate();
    }
private:
    std::thread voidF;
    std::atomic<bool> exit;
    std::future<T> future;
};

double translate(double Min,double Max, double toMin,double toMax,double value){
    return ((value-Min)/(Max-Min))*(toMax-toMin)+toMin;
}

class ImageNoise : public sf::Drawable, public EventHandler::Subscriber{
    class State{
    public:
        explicit State(ImageNoise* owner)
        : m_owner(owner)
        {}
        virtual ~State()=default;
        virtual void update()=0;
    protected:
        ImageNoise* m_owner;
    };
    class BlackWhite: public State{
    public:
        explicit BlackWhite(ImageNoise* owner)
        : State(owner)
        {}


        void update() override{
            unsigned int size=m_owner->image.getSize().x;

            float y=0;
            for(int i=0;i<size;i++) {
                float x=0;
                for (int j = 0; j <size; j++) {
                    m_owner->image.setPixel(i,j,SimplexNoise::noise(x,y,m_owner->z)>0?sf::Color::White:sf::Color::Black);
                    x+=m_owner->K;
                }
                y+=m_owner->K;
            }
            m_owner->z+=m_owner->ZK;
        }
    private:
    };
    class BlackWhiteGrad: public State{

    public:

        explicit BlackWhiteGrad(ImageNoise* owner)
        : State(owner)
        {}
        void update() override {
            unsigned int size=m_owner->image.getSize().x;

            float y=0;
            for (int i = 0; i < size; ++i) {
                float x=0;
                for (int j = 0; j < size; ++j) {
                    float noise=SimplexNoise::noise(x,y,m_owner->z);
                    int clr=(int)translate(-1,1,0,255,noise);
                    m_owner->image.setPixel(i,j,sf::Color(clr,clr,clr));
                    x+=m_owner->K;
                }
                y+=m_owner->K;
            }
            m_owner->z+=m_owner->ZK;
        }
    };
    class Red: public State{
    public:
        Red(ImageNoise* owner)
        : State(owner)
        {}

        void update() override {
            unsigned int size=m_owner->image.getSize().x;

            std::vector<std::pair<float,sf::Color>> lowerThen;
//            lowerThen.emplace_back(0.0f,sf::Color(46, 52, 56));///<DGrey
//            lowerThen.emplace_back(0.1f,sf::Color(226, 40, 64));///<Red
//            lowerThen.emplace_back(1.1f,sf::Color(95, 88, 90));///<Grey
            lowerThen.emplace_back(1.1f,sf::Color(46, 52, 56));///<DGrey
            lowerThen.emplace_back(-0.07f,sf::Color(226, 40, 64));///<Red
            lowerThen.emplace_back(-0.15f,sf::Color(95, 88, 90));///<Grey
            std::sort(lowerThen.begin(),lowerThen.end(),[](auto f,auto s){return f.first<s.first;});
            float y=0;
            for (int i = 0; i < size; ++i) {
                float x=0;
                for (int j = 0; j < size; ++j) {
                    float noise=SimplexNoise::noise(x,y,m_owner->z);
                    for(auto it=lowerThen.begin();it!=lowerThen.end();it++) {
                        if (noise < it->first) {
//                            m_owner->image.setPixel(i, j, it->second);
                            m_owner->image.setPixel(i, j, it->second);
                            break;
                        }
                        if(m_owner->thread.getExit())
                            return;
                    }
                    x+=m_owner->K;
                }
                y+=m_owner->K;
            }
            m_owner->z+=m_owner->ZK;
        }

    private:

    };
    class Terrain: public State{
    public:
        Terrain(ImageNoise* owner)
        : State(owner)
        {}
        void update() override {
            unsigned int size=m_owner->image.getSize().x;

            std::vector<std::pair<float,sf::Color>> lowerThen;
            lowerThen.emplace_back(-0.9f,sf::Color(15,94,156));///< Deep water
            lowerThen.emplace_back(-0.7f,sf::Color(35,137,218));///< Middle water
            lowerThen.emplace_back(-0.3f,sf::Color(28,163,236));///< Water
            lowerThen.emplace_back(-0.2f,sf::Color(240,238,192));///< Sand
            lowerThen.emplace_back(0.1f,sf::Color(131,106,77));///< Terrain
            lowerThen.emplace_back(0.5f,sf::Color(115,189,138));///< Forest
            lowerThen.emplace_back(0.7f,sf::Color(62,85,94));///< Mountains
            lowerThen.emplace_back(1.1f,sf::Color(241,237,229));///< Snow
            std::sort(lowerThen.begin(),lowerThen.end(),[](auto f,auto s){return f.first<s.first;});
            float y=0;
            SimplexNoise a(1,100,4);
            for (int i = 0; i < size; ++i) {
                float x=0;
                for (int j = 0; j < size; ++j) {
//                    float noise=SimplexNoise::noise(x,y,m_owner->z);
                    float noise=a.fractal(m_owner->octaves,x,y,m_owner->z);
                    for(auto it=lowerThen.begin();it!=lowerThen.end();it++) {
                        if (noise < it->first) {
                            m_owner->image.setPixel(i, j, it->second);
                            break;
                        }
                        if(m_owner->thread.getExit())
                            return;
                    }
                    x+=m_owner->K;
                }
                y+=m_owner->K;
            }
        }
    private:
    };


public:
    explicit ImageNoise(unsigned int& size, EventHandler* handler= nullptr)
    : state(new BlackWhiteGrad(this))
    , Subscriber(handler)
    , event(handler)
    , z(0)
    , ZK(0.005)
    , K(0.01)
    , slowDown(false)
    , text()
    , thread()
    , font()
    , octaves(1)
    , lastK(K)
    {
        font.loadFromFile("font.otf");
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(size/10);
        text.setOrigin(text.getGlobalBounds().width/2,text.getGlobalBounds().height/2);
        text.setPosition(size/2,size/2);


        image.create(size,size);

        timer["Zoom"]=0;
    }

    ~ImageNoise() override{
        delete state;
    }
    void update(sf::Time dt) {
        timer["Zoom"]-=timer["Zoom"]>dt.asMicroseconds()?dt.asMicroseconds():timer["Zoom"];
        if (!slowDown) {
            state->update();
        }else{
            if(thread.running() && !thread.ready()){
                text.setString("Generating...");
                text.setOrigin(text.getGlobalBounds().width/2,text.getGlobalBounds().height/2);
            }else{
                text.setString("");
                text.setOrigin(text.getGlobalBounds().width/2,text.getGlobalBounds().height/2);
            }
        }
    }
    void changeState(State* newState)
    {
        delete state;
        state=newState;
    }

    void zoom(const float& plus){
        timer["Zoom"]=2000000;
        K+=plus;
        if(K<0)
            K=0;
        if(K>1)
            K=1;
    }
    void changeZK(const float& plus){
        ZK+=plus;
        if(ZK<0)
            ZK=0;
        if(ZK>0.5)
            ZK=0.5;
    }

    void notify(const sf::Event::EventType &eventType) override {
        switch (eventType) {
            case sf::Event::KeyPressed:
                switch (event->getCode()) {
                    case sf::Keyboard::R:
                        thread.terminate();
                        changeState(new Red(this));
                        slowDown= false;
                        break;
                    case sf::Keyboard::B:
                        thread.terminate();
                        changeState(new BlackWhite(this));
                        slowDown=false;
                        break;
                    case sf::Keyboard::G:
                        thread.terminate();
                        changeState(new BlackWhiteGrad(this));
                        slowDown=false;
                        break;
                    case sf::Keyboard::T:
                        changeState(new Terrain(this));
                        slowDown=true;
                        thread.setF(&State::update,state);
                        lastK=K;
                        break;
                    case sf::Keyboard::Up:
                            if(slowDown) {
                                z += 0.1;
                                thread.setF(&State::update,state);
                                lastK=K;
                            }
                            else
                            changeZK(0.005f);
                        break;
                    case sf::Keyboard::Down:
                            if(slowDown) {
                                z -= z >= 0.1 ? 0.1 : z;
                                thread.setF(&State::update,state);
                                lastK=K;
                            }
                            else
                            changeZK(-0.005f);
                        break;
                    case sf::Keyboard::Right:
                        zoom(K!=0.f?K/10.f:0.001f);
                        break;
                    case sf::Keyboard::Left:
                        zoom(K>0.0001f?-K/10.f:-K);
                        break;
                    case sf::Keyboard::S:
                        octaves-=octaves>0?1:0;
                        thread.setF(&State::update,state);
                        lastK=K;
                        break;
                    case sf::Keyboard::A:
                        octaves+=octaves<15?1:0;
                        thread.setF(&State::update,state);
                        lastK=K;
                        break;
                }
                break;
        }
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
            if(thread.running() && !thread.ready()) {
                target.draw(text, states);
            }else{
                sf::Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                target.draw(sf::Sprite(texture), states);
                if(slowDown)
                {
                    if(timer.at("Zoom")>0) {
                        sf::RectangleShape left;
                        sf::RectangleShape top;

                        left.setFillColor(sf::Color::Red);
                        top.setFillColor(sf::Color::Red);

                        left.setSize({image.getSize().x / 50.f, image.getSize().y / 150.f});
                        top.setSize({image.getSize().x / 150.f, image.getSize().y / 50.f});

                        left.setPosition(0, K < lastK ? K / lastK * image.getSize().y : image.getSize().y -
                                                                                        left.getSize().y);
                        top.setPosition(K < lastK ? K / lastK * image.getSize().x : image.getSize().x - top.getSize().x,
                                        0);

                        target.draw(top);
                        target.draw(left);
                    }
                }
            }
    }

private:
    sf::Image image;
    sf::Text text;
    sf::Font font;
    std::map<std::string,int64_t> timer;
    int octaves;

    ThreadCrutch<void> thread;

    State* state;

    EventHandler* event;

    float lastK;
    float z;
    float K;
    float ZK;

    bool slowDown;
};
 class A{
public:
     void foo(){
        while (true) {
            std::cout<<T;
            if (crutch.getExit())
                return;
        }
    }
    A(int t):T(t){
        crutch.setF(&A::foo,this);
    }

    ~A(){
        crutch.terminate();
    }
private:
    ThreadCrutch<void> crutch;
     int T;
};

 int foo()
 {
     std::cout<<1<<std::endl;
     std::this_thread::sleep_for(std::chrono::seconds(5));
 }

int main(int args,char **argv)
{
    unsigned int ScreenSize=(unsigned)(sf::VideoMode::getFullscreenModes().front().height/1.5);
    sf::RenderWindow window(sf::VideoMode(ScreenSize,ScreenSize),"SimplexNoise");
    window.setFramerateLimit(50);

    EventHandler handler(&window);

    ImageNoise noise(ScreenSize,&handler);
    handler.Subscribe(&noise,1,sf::Event::KeyPressed);
    sf::Clock clock;
    sf::Time time;
    while (window.isOpen())
    {
        time=clock.restart();
        handler.pollEvent();
        if(handler.happened(sf::Event::Closed))
            window.close();
        noise.update(time);
        window.clear();
        window.draw(noise);
        window.display();
    }
     /*
    std::packaged_task<void()> task([](){std::this_thread::sleep_for(std::chrono::seconds(1));});
    std::future<void> f=task.get_future();
    std::thread thread(std::move(task));

    while(f.wait_for(std::chrono::seconds(0))!=std::future_status::ready)
        std::cout<<"NO"<<std::endl;

    thread.join();
     */
//    A a(10);
//    std::this_thread::sleep_for(std::chrono_literals::operator""s(1.0L));

    return 0;
}

