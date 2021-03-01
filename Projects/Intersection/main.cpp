#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdarg>
#include <stack>
#include <cmath>
#include <map>
#include <set>

#define PI 3.14159265359

template<typename T,typename TT>
inline bool operator<(sf::Vector2<T> first,sf::Vector2<TT> second){
    return abs(first.x)+abs(first.y)<abs(second.x)+abs(second.y);
}



template<typename T>
inline sf::Vector2<T> abs(const sf::Vector2<T>& vector){
    return {std::abs(vector.x),std::abs(vector.y)};
}

sf::Vector2f GetPoint(const size_t &index, const int &radius, const float &count) {
    static const float pi = 3.141592654f;

    float angle = index * 2.f * pi / count - pi / 2.f;
    float x = cos(angle) * radius;
    float y = sin(angle) * radius;

    return {radius + x, radius + y};
}

bool inCircle(const sf::Vector2i& point, const sf::Vector2i& pos, const int& radius){  // 19.1, 19.1, 19.1
    int dx = abs(point.x-pos.x);
    if (    dx >  radius ) return false; // \*
    int dy = abs(point.y-pos.y);         // square around a circle
    if (    dy >  radius ) return false; // /*
    if ( dx+dy <= radius ) return true;  // rhombus around the circle
    return ( dx*dx + dy*dy <= radius*radius ); //If nothing happened-Pythagoras
}

long unsigned int randn(const long& n){
    if(n>LONG_MAX || n<=0)
        throw std::out_of_range("Rand value out of range");

    const long bucket=LONG_MAX/n;
    long unsigned int r;
    const static int intbit =sizeof(int) *8;
    const static int longbit=sizeof(long)*8;
    const static int dif    =longbit-intbit;
    int curDif=dif;
    do{
        long unsigned int random=rand();
        long unsigned int* ptr  =&random;

        int i=1;
        while(curDif>intbit)
        {
            int t=rand();
            memcpy(ptr+sizeof(int)*i,&t,1);
            curDif-=intbit;
            i++;
        }
        random<<=curDif;
        random|=rand()%(int)pow(2,curDif);

         r = random / bucket;
    }while (r>=n);
    return r;
}

void line(int x0, int y0, int x1, int y1, sf::Image& image,const sf::Color& color) {
    if(x0>x1)
        std::swap(x0,x1);
    if(y0>y1)
        std::swap(y0,y1);
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    int error = 0;
    int deltaerr = (deltay + 1);
    int y = y0;
    int diry = y1 - y0;
    if( diry > 0)
        diry = 1;
    if (diry < 0)
        diry = -1;
    for(int x=x0;x<x1;x++) {
        image.setPixel(x,y,color);// Надо увеличивать фигуру чтоб рисовать
        error = error + deltaerr;
        if (error >= (deltax + 1)) {
            y = y + diry;
            error = error - (deltax + 1);
        }
    }
}

void BresenhamCircle(std::deque<sf::Vector2i>& array, const int& _x, const int& _y, const int& radius)
{
    int x = 0, y = radius, gap = 0, delta = (2 - 2 * radius);
    while (y >= 0)
    {
            array.emplace_back(_x + x, _y + y);//RightDown
            array.emplace_back( _x + x, _y - y);//RightUp
            array.emplace_back(_x - x, _y - y);//LeftUp
            array.emplace_back(_x - x, _y + y);//LeftDown
        gap = 2 * (delta + y) - 1;
        if (delta < 0 && gap <= 0)
        {
            x++;
            delta += 2 * x + 1;
            continue;
        }
        if (delta > 0 && gap > 0)
        {
            y--;
            delta -= 2 * y + 1;
            continue;
        }
        x++;
        delta += 2 * (x - y);
        y--;
    }
}

void fill(sf::Image& image,const sf::Vector2i& pos,const sf::Color& oldColor,const sf::Color& newColor)
{
    const sf::Vector2u size=image.getSize();
    std::stack<sf::Vector2i> fill;
    fill.push(pos);
    while(!fill.empty())
    {
        sf::Vector2i temp=fill.top();
        fill.pop();
        bool CheckedLineDown = false;
        bool CheckedLineUp = false;

        while(temp.x>0 && image.getPixel(temp.x-1,temp.y)==oldColor)
            --temp.x;

        while(temp.x<size.x && image.getPixel(temp.x,temp.y)==oldColor) {
            image.setPixel(temp.x,temp.y,newColor);

            if(temp.y>0) {
                if(!CheckedLineUp) {
                    if (image.getPixel(temp.x, temp.y -1) == oldColor) {
                        CheckedLineUp = true;
                        fill.push({temp.x, temp.y - 1});
                    }
                } else {
                    if (image.getPixel(temp.x, temp.y - 1) != oldColor)
                        CheckedLineUp = false;
                }
            }


            if(temp.y+1<size.y)
            {
                if(!CheckedLineDown) {
                    if (image.getPixel(temp.x, temp.y + 1) == oldColor) {
                        CheckedLineDown = true;
                        fill.push({temp.x, temp.y + 1});
                    }
                } else {
                    if (image.getPixel(temp.x, temp.y + 1) != oldColor)
                        CheckedLineDown = false;
                }
            }

            ++temp.x;
        }
    }
}
/*
void thickLine(int x0, int y0, int x1, int y1,unsigned int thickness, sf::Image& image, const sf::Color& color) {
    if(x0>x1)
        std::swap(x0,x1);
    if(y0>y1)
        std::swap(y0,y1);
    const sf::Vector2u size=image.getSize();
    const unsigned int devided=thickness/2U;

    int firstLineX0=x0-devided>=0?x0-(int)devided:0;
    int firstLineY0=y0-devided>=0?x0-(int)devided:0;

    int firstLineX1=x1+devided<size.x?x1+(int)devided:(int)size.x;
    int firstLineY1=y1+devided<size.y?x1+(int)devided:(int)size.y;

        int deltax = x1 - x0;
        int deltay = y1 - y0;
        int error = 0;
        int deltaerr = (deltay + 1);
        int y = y0;
        int diry = y1 - y0;
        if (diry > 0)
            diry = 1;
        if (diry < 0)
            diry = -1;
        for (int x = x0; x < x1; x++) {
            image.setPixel(x, y, color);// Надо увеличивать фигуру чтоб рисовать
            error = error + deltaerr;
            if (error >= (deltax + 1)) {
                y = y + diry;
                error = error - (deltax + 1);
            }
        }

    int secondLineX=
    int secondLineY=
}
 */


class EventHandler{
public:
    class Subscriber{
    public:
        explicit Subscriber(EventHandler* newHandler)
        : toClear(newHandler)
        {}
        ~Subscriber(){
            toClear->clearSub(this);
        }
        virtual void notify(const sf::Event::EventType& eventType)=0;

    private:
        EventHandler* toClear;
    };


    explicit EventHandler(sf::Window* setWindow)
    : event{}
    {
        window= setWindow;
    }

    [[nodiscard]] bool happened(const sf::Event::EventType& eventType) const{
        try {
            return events.at(eventType);
        } catch (std::exception& dontExist) {
            return false;
        }
    }

    const sf::Event& getEvent(){
        return event;
    }

    void pollEvent(){
        for(std::map<sf::Event::EventType,bool>::iterator it=events.begin();it!=events.end();it++)
            it->second=false;
        while (window->pollEvent(event)){
            for(std::multimap<sf::Event::EventType,Subscriber*>::iterator it=subscribers.lower_bound(event.type);
                it!=subscribers.upper_bound(event.type);it++)
                    it->second->notify(event.type);

            events[event.type]=true;
        }
    }
    void waitEvent(){
        for(std::map<sf::Event::EventType,bool>::iterator it=events.begin();it!=events.end();it++)
            it->second=false;
        if(window->waitEvent(event)){
            for(std::multimap<sf::Event::EventType,Subscriber*>::iterator it=subscribers.lower_bound(event.type);
                it!=subscribers.upper_bound(event.type);it++)
                    it->second->notify(event.type);

            events[event.type]=true;
        }
    }

    void Subscribe(Subscriber* newSub,unsigned int num,...){
        va_list list;
        va_start(list,num);
        for(int i=0;i<num;i++)
            subscribers.emplace((sf::Event::EventType)va_arg(list,int),newSub);
        va_end(list);
    }

    void Subscribe(const sf::Event::EventType& listenTo,Subscriber* newSub)
    {
        subscribers.emplace(listenTo,newSub);
    }


    void UnSubscribe(Subscriber* oldSub,unsigned int num,...){
        va_list list;
        va_start(list,num);
        std::set<sf::Event::EventType> subscriptions;
        for(int i=0;i<num;i++)
            subscriptions.emplace((sf::Event::EventType)va_arg(list,int));
        va_end(list);
            for(std::multimap<sf::Event::EventType,Subscriber*>::iterator it=subscribers.begin();
                it!=subscribers.end();)
                    if(it->second==oldSub && subscriptions.find(it->first)!=subscriptions.end()) {
                        subscriptions.erase(it->first);
                        it=subscribers.erase(it);
                        if(subscriptions.empty())
                            return;
                    }else
                        it++;
    }

    void UnSubscribe(const sf::Event::EventType& listenedTo,Subscriber* oldSub){
        for(std::multimap<sf::Event::EventType,Subscriber*>::iterator it=subscribers.begin();
            it!=subscribers.end();it++)
            if(it->first==listenedTo && it->second==oldSub)
            {
                subscribers.erase(it);
                return;
            }
    }
    void clearSub(Subscriber* deletingSub){
        for(std::multimap<sf::Event::EventType,Subscriber*>::iterator it=subscribers.begin();
            it!=subscribers.end();)
            if(it->second==deletingSub)
                it=subscribers.erase(it);
            else
                it++;
    }
private:
    sf::Event event;
    sf::Window* window;
    std::multimap<sf::Event::EventType,Subscriber*> subscribers;
    std::map<sf::Event::EventType,bool> events;
};

class App: public EventHandler::Subscriber{
public:
    class State
            : public EventHandler::Subscriber
            , public sf::Drawable{
    public:
        explicit State(App* New)
        : app(New)
        , Subscriber(&New->Manager)
        {}
        ~State() override =default;
        virtual void update()=0;
        void notify(const sf::Event::EventType &eventType) override =0;
    protected:
        App* app;
    };



    /*///////////////////////////////////////////////////////////////////////*/
    class MainProcces: public State{
    class State: public sf::Drawable{
        public:
            explicit State(App::MainProcces* owner)
            : owner(owner)
            , holding(nullptr)
            , touched(false)
            {
                font.loadFromFile("font.otf");
                area.setFont(font);
                area.setPosition(10,10);
                area.setCharacterSize(40);
                area.setString("Feel free to get started");
            }

            void onClick(){
                sf::ConvexShape* shape=contains(sf::Mouse::getPosition(*owner->app->window));
                if(shape==nullptr)
                {
                    touch=sf::Mouse::getPosition(*owner->app->window);
                    touched=true;
                    newShape();
                }else{
                    holding=shape;
                    holding->setOrigin(sf::Vector2f(sf::Mouse::getPosition(*owner->app->window))-holding->getPosition());
                }
            }

            void update(){
                if(holding!= nullptr)
                    holding->setPosition(sf::Vector2f(sf::Mouse::getPosition(*owner->app->window)));
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && owner->app->Manager.happened(sf::Event::MouseMoved))
                    updateShape();
                if(owner->app->Manager.happened(sf::Event::MouseMoved) || holding!= nullptr)
                    area.setString("Area of intersection is:"+std::to_string(   intersection())+" pixels");
            }


            void onRelease(){
                if(holding!= nullptr) {
                    holding->setPosition(holding->getPosition()-holding->getOrigin());
                    holding->setOrigin(0,0);
                    holding = nullptr;
                }
                if(touched)
                {
                    if(touch==sf::Mouse::getPosition(*owner->app->window) && !shapes.empty())
                        shapes.pop_back();
                    if(shapes.size()>2)
                        shapes.pop_front();
                }


            }

            virtual void newShape()=0;
            virtual void updateShape()=0;
            virtual int intersection()=0;
            virtual sf::ConvexShape* contains(const sf::Vector2i& pos)=0;


        ~State() override=default;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
            for(const sf::ConvexShape& shape:shapes)
                target.draw(shape, states);
            target.draw(intersectionZone,states);
            target.draw(area,states);
        }

            std::deque<sf::ConvexShape> shapes;
            sf::ConvexShape* holding;
            sf::ConvexShape intersectionZone;

            sf::Color last;

            sf::Text area;
            sf::Font font;

            sf::Vector2i touch;
            bool touched;

            App::MainProcces* owner;
        };


    class Rects: public App::MainProcces::State{
    public:
        explicit Rects(App::MainProcces* owner)
        : State(owner)
        {}

        void newShape() override{
            shapes.emplace_back();
            shapes.back().setOutlineThickness(2);
            if(last==sf::Color(128,128,128)) {
                shapes.back().setFillColor({180, 140, 220});
                shapes.back().setOutlineColor(sf::Color::Blue);
                last={180, 140, 220};
            }
            else {
                shapes.back().setFillColor({128, 128, 128});
                shapes.back().setOutlineColor(sf::Color::Cyan);
                last={128,128,128};
            }
            shapes.back().setPosition(sf::Vector2f (touch));
            shapes.back().setPointCount(4);
        }

        int intersection() override {
            if(shapes.size()>=2) {
                sf::FloatRect first = shapes[0].getGlobalBounds();
                sf::FloatRect second = shapes[1].getGlobalBounds();

                int left = std::max(first.left, second.left);
                int top = std::max(first.top, second.top);
                int right = std::min(first.left + first.width, second.left + second.width);
                int bottom = std::min(first.top + first.height, second.top + second.height);

                int width = right - left;
                int height = bottom - top;

                if (width>0 && height>0) {
                    intersectionZone.setPointCount(4);
                    intersectionZone.setPoint(0,sf::Vector2f(left,top));
                    intersectionZone.setPoint(1,sf::Vector2f(right,top));
                    intersectionZone.setPoint(2,sf::Vector2f(right,bottom));
                    intersectionZone.setPoint(3,sf::Vector2f(left,bottom));
                    return width * height;
                }
            }
            intersectionZone.setPointCount(0);
            return 0;
        }

        void updateShape() override{
            sf::Vector2i cur=sf::Mouse::getPosition(*owner->app->window);

            shapes.back().setPoint(0,{0,0});
            shapes.back().setPoint(1,sf::Vector2f (cur.x-touch.x,0));
            shapes.back().setPoint(2,sf::Vector2f (cur.x-touch.x,cur.y-touch.y));
            shapes.back().setPoint(3,sf::Vector2f (0,cur.y-touch.y));
        }

        sf::ConvexShape *contains(const sf::Vector2i &pos) override {
            for(sf::ConvexShape& shape:shapes)
                if(shape.getGlobalBounds().contains(sf::Vector2f(pos)))
                    return &shape;
                return nullptr;
        }
    };

    class Circle: public App::MainProcces::State{
        public:
            explicit Circle(App::MainProcces* owner)
                    : State(owner)
            {}

            void newShape() override{
                shapes.emplace_back();
                if(last==sf::Color(128,128,128)) {
                    shapes.back().setFillColor({180, 140, 220});
                    last={180, 140, 220};
                }
                else {
                    shapes.back().setFillColor({128, 128, 128});
                    last={128,128,128};
                }
                shapes.back().setPosition(sf::Vector2f (touch));
            }

            int intersection() override {
                if(shapes.size()>=2) {
                    const float fR=shapes[0].getGlobalBounds().height/2;
                    const float sR=shapes[1].getGlobalBounds().height/2;

                    const sf::Vector2f pos1=shapes[0].getPosition();
                    const sf::Vector2f pos2=shapes[1].getPosition();

                    const sf::Vector2f orig1=shapes[0].getOrigin();
                    const sf::Vector2f orig2=shapes[1].getOrigin();

                    const sf::Vector2f P1=pos1-orig1+sf::Vector2f(fR,fR);
                    const sf::Vector2f P2=pos2-orig2+sf::Vector2f(sR,sR);

                    const sf::Vector2f distance=abs(P1-P2);
                    const double D=sqrt(pow(distance.x,2)+pow(distance.y,2));


                    if(D>fR+sR) {
                        intersectionZone.setPointCount(0);
                        return 0;
                    }
                    if(D<=std::abs(fR-sR)) {
                        if(fR<sR) {
                            intersectionZone=shapes[0];
                            intersectionZone.setPosition(pos1-orig1);
                            intersectionZone.setFillColor(sf::Color::White);
                            intersectionZone.setOrigin(0,0);
                        }else{
                            intersectionZone=shapes[1];
                            intersectionZone.setPosition(pos2-orig2);
                            intersectionZone.setFillColor(sf::Color::White);
                            intersectionZone.setOrigin(0,0);
                        }
                        return fR < sR ? pow(fR, 2) * PI : pow(sR, 2) * PI;
                    }


                    const double fromP1toHP = (fR * fR - sR * sR + D * D) / (2.0 * D);

                    const double h=sqrt(fR*fR-fromP1toHP*fromP1toHP);

                    const sf::Vector2f HP=sf::Vector2f (P1+(float)fromP1toHP/(float)D*(P2-P1));

                    const sf::Vector2f resUp  =sf::Vector2f(HP.x+h/D*(P2.y-P1.y),HP.y-h/D*(P2.x-P1.x));
                    const sf::Vector2f resDown=sf::Vector2f(HP.x-h/D*(P2.y-P1.y),HP.y+h/D*(P2.x-P1.x));


                        std::deque<sf::Vector2i> points;
                        {
                            int Upindex = 0;
                            int Downindex = 0;
                            sf::Vector2i Uclosest = {0, INT_MAX};
                            sf::Vector2i Dclosest = {0, INT_MAX};


                            for (int i = 0; i < shapes[0].getPointCount(); i++) {
                                sf::Vector2i point = sf::Vector2i(shapes[0].getPoint(i));
                                if (abs(point - sf::Vector2i(resUp) + sf::Vector2i(pos1 - orig1)) < Uclosest) {
                                    Upindex = i;
                                    Uclosest = abs(point - sf::Vector2i(resUp) + sf::Vector2i(pos1 - orig1));
                                }
                                if (abs(point + sf::Vector2i(pos1 - orig1) - sf::Vector2i(resDown)) < Dclosest) {
                                    Downindex = i;
                                    Dclosest = abs(point - sf::Vector2i(resDown) + sf::Vector2i(pos1 - orig1));
                                }
                            }
                            if(Upindex!=Downindex) {
                                int &startIndex = Upindex;
                                int &endIndex = Downindex;

                                for (int i = startIndex + 1; i != endIndex; i++) {
                                    if (i == shapes[0].getPointCount())
                                        i = -1;
                                    else
                                        points.emplace_back(shapes[0].getPoint(i) + pos1 - orig1);
                                }
                                points.emplace_front(resUp);
                                points.emplace_back(resDown);
                            }
                        }{
                            int Upindex = 0;
                            int Downindex = 0;
                            sf::Vector2i Uclosest = {0, INT_MAX};
                            sf::Vector2i Dclosest = {0, INT_MAX};


                            for (int i = 0; i < shapes[1].getPointCount(); i++) {
                                sf::Vector2i point = sf::Vector2i(shapes[1].getPoint(i));
                                if (abs(point - sf::Vector2i(resUp) + sf::Vector2i(pos2 - orig2)) < Uclosest) {
                                    Upindex = i;
                                    Uclosest = abs(point - sf::Vector2i(resUp) + sf::Vector2i(pos2 - orig2));
                                }
                                if (abs(point - sf::Vector2i(resDown) + sf::Vector2i(pos2 - orig2)) < Dclosest) {
                                    Downindex = i;
                                    Dclosest = abs(point - sf::Vector2i(resDown) + sf::Vector2i(pos2 - orig2));
                                }
                            }
                                if(Upindex!=Downindex) {
                                    int &startIndex = Upindex;
                                    int &endIndex = Downindex;
                                    std::swap(startIndex, endIndex);

                                    for (int i = startIndex + 1; i != endIndex; i++) {
                                        if (i == shapes[1].getPointCount())
                                            i = -1;
                                        else
                                            points.emplace_back(shapes[1].getPoint(i) + pos2 - orig2);
                                    }
                                }
                        }
                        intersectionZone.setPosition(0, 0);
                        if (!points.empty()) {
                            points.emplace_back(points.front());
                            intersectionZone.setPointCount(points.size());
                            for (int i = 0; i < intersectionZone.getPointCount(); i++) {
                                intersectionZone.setPoint(i, sf::Vector2f(points.front()));
                                points.pop_front();
                            }
                        }


                    const double F1=2.0*acos((fR*fR-sR*sR+D*D)/(2.0*fR*D));
                    const double F2=2.0*acos((sR*sR-fR*fR+D*D)/(2.0*sR*D));

                    const double S1=(fR*fR*(F1-sin(F1)))/2.0;
                    const double S2=(sR*sR*(F2-sin(F2)))/2.0;

                    return S1+S2;
                }
                return 0;
            }

            void updateShape() override{
                sf::Vector2i cur=sf::Mouse::getPosition(*owner->app->window);

                sf::Vector2f temp=sf::Vector2f (cur-touch);
                int pointsCount=60;
                shapes.back().setPointCount(pointsCount+1);


                for(int i=0;i<pointsCount;i++)
                    shapes.back().setPoint(i,GetPoint(i,temp.x,pointsCount));
                shapes.back().setPoint(pointsCount,GetPoint(0,temp.x,pointsCount));
            }

            sf::ConvexShape *contains(const sf::Vector2i &pos) override {
                for(sf::ConvexShape& shape:shapes)
                        if(inCircle(pos,sf::Vector2i(shape.getPosition().x+shape.getGlobalBounds().width/2,
                                             shape.getPosition().y+shape.getGlobalBounds().height/2),
                                             shape.getGlobalBounds().width/2))
                            return &shape;
                return nullptr;
            }
        };
    /*///////////////////////////////////////////////////////////////////////*/

    public:
        explicit MainProcces(App* New)
        : App::State(New)
        , state(new Circle(this))
        , Rstate(true)
        {
            app->Manager.Subscribe(this,3,sf::Event::EventType::MouseButtonPressed
                                                      ,sf::Event::EventType::MouseButtonReleased
                                                      ,sf::Event::EventType::KeyPressed);
        }
        ~MainProcces(){
            delete state;
        }

        void notify(const sf::Event::EventType &eventType) override {
            switch (eventType) {
                default:
                    break;
                case sf::Event::MouseButtonPressed:
                    if(app->window->hasFocus())
                        onClick();
                    break;
                case sf::Event::MouseButtonReleased:
                    if(app->window->hasFocus())
                        onRelease();
                    break;
                case sf::Event::KeyPressed:
                    if(app->window->hasFocus())
                        switch (app->Manager.getEvent().key.code) {
                            case sf::Keyboard::LShift:
                            if(Rstate) {
                                changeState(new Circle(this));
                                Rstate=false;
                            }else{
                                changeState(new Rects(this));
                                Rstate=true;
                            }
                            break;
                    }
                    break;
            }
        }

        void changeState(State* New){
            delete state;
            state=New;
        }

        void update() override {
            state->update();
        }

        void onClick(){
            state->onClick();
        }
        void onRelease(){
            state->onRelease();
        }
    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
            target.clear();
            target.draw(*state,states);
        }

    private:
        MainProcces::State* state;
        bool Rstate;
    };
    /*/////////////////////////////////////////////////////////////////*/




    explicit App(sf::RenderWindow* window=new sf::RenderWindow(sf::VideoMode(200,200),"SFML works!"))
    : window(window)
    , Manager(window)
    , Subscriber(&Manager)
    {
        addState(new MainProcces(this));
        window->setFramerateLimit(40);
    }
    ~App(){
        delete window;
        while(!layers.empty())
        removeState();
    }
    void render() const{
        window->draw(*layers.top());
        window->display();
    }

    void run(){
        while (window->isOpen()){
            Manager.pollEvent();
            if(layers.empty() || Manager.happened(sf::Event::EventType::Closed))
                window->close();
            layers.top()->update();
            render();
        }
    }

    void notify(const sf::Event::EventType &eventType) override {

    }

    void removeState(){
        delete layers.top();
        layers.pop();
    }
    void addState(State* newState){
        layers.push(newState);
    }
    EventHandler& EventSystem(){
        return Manager;
    }
private:
    sf::RenderWindow* window;
    EventHandler Manager;
    std::stack<State*> layers;
};

int main(int args,char** argv)
{
    srand(std::clock());
    App a(new sf::RenderWindow(sf::VideoMode(1000,1000),"SFML",sf::Style::Titlebar));
    a.run();
    return 0;
}