#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Angry bird");
    // Enable the event Filter
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));
    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());
    // Create ground (You can edit here)
    itemList.push_back(new Land(16,1,32,1.2,QPixmap(":/ground.png").scaled(width(),height()/6.0),world,scene));
    itemList.push_back(new Land(32,10,1,18,QPixmap(":/ground.png").scaled(width()/32*1,height()/18*18),world,scene));

    // Setting background
    QPixmap background;
    background.load(":/bgb.png");
    background = background.scaled(1000,600);
    gameBg = new QGraphicsPixmapItem;
    gameBg->setPixmap(background);
    gameBg->setPos(0,0);
    scene->addItem(gameBg);

    /*// Create bird (You can edit here)
    Bird *birdie = new Bird(0.0f,10.0f,0.27f,&timer,QPixmap(":/bird.png").scaled(height()/9.0,height()/9.0),world,scene);
    // Setting the Velocity
    birdie->setLinearVelocity(b2Vec2(5,5));
    itemList.push_back(birdie);*/
    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);
    // Setting restart button
    restart = new QPushButton(this);
    restart->setObjectName("re");
    restart->setGeometry(850,0,80,80);
    restart->setStyleSheet("#re{border-image: url(:/re.png)}");
    restart->show();

    exit = new QPushButton(this);
    exit->setObjectName("exit");
    exit->setGeometry(750,0,80,80);
    exit->setStyleSheet("#exit{border-image: url(:/exit.png)}");
    exit->show();

    // Setting catapult
    QPixmap catapult;
    catapult.load(":/catapult.png");
    catapult = catapult.scaled(48,108);
    gameCatapult = new QGraphicsPixmapItem;
    gameCatapult->setPixmap(catapult);
    gameCatapult->setPos(160,360);
    scene->addItem(gameCatapult);

    // Setting score board
    scoreBoard = new QLabel(this);
    scoreBoard->setFont(QFont("BlackMagic", 15, QFont::Bold));
    scoreBoard->setText("<h1><font color=black>Score : " + QString::number(score));
    scoreBoard->setGeometry(20,0,300,100);
    scoreBoard->show();

    // Setting bgm
    bgm = new QMediaPlayer;
    bgm->setMedia(QUrl("qrc:/bgm.mp3"));
    bgm->play();

    // Starting game
    startGame();

}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    // Hint: Notice the Number of every event!
    QMouseEvent *mouseplace = static_cast<QMouseEvent*>(event);
    QPointF place = mouseplace->pos();
    QMouseEvent *mouse = static_cast<QMouseEvent*>(event);

    if(event->type() == QEvent::MouseButtonPress && playGame == true)
    {
        /* TODO : add your code here */
        // Adding restart event
        if((x>=28.5f && x<=31.0f) && (y>=15.5f && y<=18.0f))
        {
            restartGame();
            startGame();
            return true;
        }

        if((x>=24.5f && x<=27.0f) && (y>=16.5f && y<=19.0f))
        {
            emit quitGame();
            QApplication::quit();
        }

        if(bird[birdType]->clicked==true && mouse->button()==Qt::LeftButton && bird[birdType]->useTimes>0)
        {
            bird[birdType]->skill();
            bird[birdType]->useTimes--;

            return true;
        }

        if(bird[birdType]->control==false && r<0.5 && bird[birdType]->clicked==false && mouse->button()==Qt::LeftButton)
        {
            bird[birdType]->control = true;

            pressX = (place.x()*32)/960;
            pressY = (place.y()/540-1)*18*-1;

            bird[birdType]->clicked = true;
            return true;
        }

        if (bird[birdType]->clicked==true && mouse->button()==Qt::RightButton && birdType < 3)
        {
            b2Vec2 move = bird[birdType]->g_body->GetLinearVelocity();
            if(move.x<0.1 && move.y<0.1)
            {
                birdType++;
                if(birdType == 1)
                {
                    bird[birdType] = new Bluebird(5.9f, 5.7f, 0.5, &timer, QPixmap(":/blue.png").scaled(width()/32*1,height()/18*1),world,scene);
                }
                if(birdType == 2)
                {
                    bird[birdType] = new Yellowbird(5.9f, 5.7f, 0.7, &timer, QPixmap(":/yellow.png").scaled(width()/32*1.4,height()/18*1.4),world,scene);
                }
                if(birdType == 3)
                {
                    bird[birdType] = new Greenbird(5.9f, 5.7f, 0.7, &timer, QPixmap(":/green.png").scaled(width()/32*2.0,height()/18*1.4),world,scene);
                }
                return true;
            }
        }
        /*std::cout << "Press !" << std::endl ;*/
    }
    if(event->type() == QEvent::MouseMove && playGame == true)
    {
        /* TODO : add your code here */
        //std::cout << "Move !" << std::endl ;
        x = (place.x()*32)/960;
        y = (place.y()/540 - 1)*18*-1;
        r = sqrt((x - bird[birdType]->startx)*(x - bird[birdType]->startx) + (y - bird[birdType]->starty)*(y - bird[birdType]->starty));

        if(bird[birdType]->control==true && bird[birdType]->clicked==true)
        {
            double birdX = bird[birdType]->g_body->GetPosition().x;
            double birdY = bird[birdType]->g_body->GetPosition().y;
            double rangeX = x - birdX;
            double rangeY = y - birdY;
            double boundaryX = (birdX + rangeX - pressX);
            double boundaryY = (birdY + rangeY - pressY);
            double boundary = sqrt(boundaryX*boundaryX + boundaryY*boundaryY);

            if(boundary <= 3)
            {
                b2Vec2 sp(birdX+rangeX, birdY+rangeY);
                bird[birdType]->g_body->SetTransform(sp,0);
            }
        return true;
        }
   return true;
    }
    if(event->type()==QEvent::MouseButtonRelease && playGame == true)
    {
        /* TODO : add your code here */
        //std::cout << "Release !" << std::endl ;

        if(bird[birdType]->control==true)
        {
            wee = new QMediaPlayer;
            wee->setMedia(QUrl("qrc:/wee.mp3"));
            wee->play();

            double rangeX = x - pressX;
            double rangeY = y - pressY;
            bird[birdType]->setLinearVelocity(b2Vec2(5*rangeX*-1,5*rangeY*-1));
            bird[birdType]->control = false;
            return true;
        }
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::pigHit()
{
    timecount++;

    if(timecount%100 == 0)
    {
        pig1_vx = pig[0]->g_body->GetLinearVelocity().x;
        pig1_vy = pig[0]->g_body->GetLinearVelocity().y;
        pig1_angle = pig[0]->g_body->GetAngularVelocity();
    }
    if(timecount%100 == 0)
    {
        pig2_vx = pig[1]->g_body->GetLinearVelocity().x;
        pig2_vy = pig[1]->g_body->GetLinearVelocity().y;
        pig2_angle = pig[1]->g_body->GetAngularVelocity();
    }

    if(pig1_vx>0.1 || pig1_vx<-0.1 || pig2_vx>0.1 || pig2_vx<-0.1)
    {
        score += 87;
        scoreBoard->setText("<h1><font color=black>Score : " + QString::number(score));
    }

    if(pig1_vy>0.1 || pig2_vy>0.1)
    {
        score += 69;
        scoreBoard->setText("<h1><font color=black>Score : " + QString::number(score));
    }

    if(pig1_angle>1 || pig1_angle<-1 || pig2_angle>1 || pig2_angle<-1)
    {
        score += 99;
        scoreBoard->setText("<h1><font color=black>Score : " + QString::number(score));
    }
}

void MainWindow::startGame()
{
    // Creating birds
    birdType = 0;
    bird[0] = new Bird(5.9f, 5.7f, 0.6, &timer, QPixmap(":/bird.png").scaled(width()/32*1.2,height()/18*1.2),world,scene);
    // Creating pigs
    pig[0] = new Bird(23.2f, 5.0f, 1, &timer, QPixmap(":/pig.png").scaled(width()/32.0*2,height()/18.0*2), world, scene);
    pig[1] = new Bird(23.0f, 10.15f, 0.75, &timer, QPixmap(":/pig.png").scaled(width()/32.0*1.5,height()/18.0*1.5), world, scene);
    pig[0]->setLinearVelocity(b2Vec2(0,0.1));
    pig[1]->setLinearVelocity(b2Vec2(0,0.1));
    // Creating bottom obstacles
    building[0] = new Obstacle(20.0f, 5.5f, 0.75, 3, &timer, QPixmap(":/vertical_obstacle.png").scaled(width()/32.0*0.75,height()/18.0*3), world, scene);
    building[1] = new Obstacle(26.0f, 5.5f, 0.75, 3, &timer, QPixmap(":/vertical_obstacle.png").scaled(width()/32.0*0.75,height()/18.0*3), world, scene);
    building[2] = new Obstacle(23.1f, 9.0f, 7.5, 0.5, &timer, QPixmap(":/parallel_obstacle.png").scaled(width()/32.0*7.5,height()/18.0*0.5), world, scene);
    // Creating upper obstacles
    building[3] = new Obstacle(21.0f, 9.25f, 0.5, 2.5, &timer, QPixmap(":/vertical_obstacle.png").scaled(width()/32.0*0.5,height()/18.0*2.5), world, scene);
    building[4] = new Obstacle(25.0f, 9.25f, 0.5, 2.5, &timer, QPixmap(":/vertical_obstacle.png").scaled(width()/32.0*0.5,height()/18.0*2.5), world, scene);
    building[5] = new Obstacle(23.15f, 12.5f, 5, 0.5, &timer, QPixmap(":/parallel_obstacle.png").scaled(width()/32.0*5,height()/18.0*0.5), world, scene);

    playGame = true;
}

void MainWindow::restartGame()
{
    score = 0;
    scoreBoard->setText("<h1><font color=black>Score : " + QString::number(score));
    timecount = 0;

    // Delete everything
    for (int i = 0 ; i <= birdType ; i++)
    {
        delete bird[i];
    }

    delete pig[0];
    delete pig[1];

    delete building[0];
    delete building[1];
    delete building[2];
    delete building[3];
    delete building[4];
    delete building[5];


    playGame = false;
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);
    pigHit();
    scene->update();
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}
