#include "mygraphicview.h"
#include "ui_mygraphicview.h"
#include <iostream>

#define BORDER 20
MyGraphicView::MyGraphicView(QWidget *parent) :
     QGraphicsView(parent),
    ui(new Ui::MyGraphicView)
{
    /* Немного поднастроим отображение виджета и его содержимого */
      this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
      this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
      //this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
      this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

      /* Также зададим минимальные размеры виджета
       * */
      //this->setMinimumHeight(100);
      //this->setMinimumWidth(100);

    scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    this->setScene(scene);          // Устанавливаем сцену в виджет

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_2 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов
    group_3 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов

    scene->addItem(group_1);            // Добавляем первую группу в сцену
    scene->addItem(group_2);            // Добавляем вторую группу в сцену
    scene->addItem(group_3);            // Добавляем вторую группу в сцену

    timer = new QTimer();               // Инициализируем Таймер
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(50);                   // Стартуем таймер на 50 миллисекунд

/* QPainter paint(this);
         //paint = new QPainter;
    //QPainter painter1; //создание рисовальщика
    paint.begin(this); //захват контекста
    paint.fillRect(0,0,width(),height(),Qt::CrossPattern); //отрисовка
    paint.end();//освобождение контекста*/
    ui->setupUi(this);
}
/*void MyGraphicView::paintEvent(QPaintEvent *event)
{
    //QPainter paint(this);
    //paint = new QPainter(this);
    //paint->begin(this); //захват контекста

    //paint.fillRect(0,0,width(),height(),Qt::CrossPattern); //отрисовка
    //paint->end();//освобождение контекста
}*/
/* Этим методом перехватываем событие изменения размера виджет
 * */
void MyGraphicView::resizeEvent(QResizeEvent *event)
{
    timer->start(50);   // Как только событие произошло стартуем таймер для отрисовки
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
}
void MyGraphicView::slotAlarmTimer()
{
    if(ans_data)
    {
        /* Удаляем все элементы со сцены,
         * если они есть перед новой отрисовкой
         * */
        this->deleteItemsFromGroup(group_1);
        this->deleteItemsFromGroup(group_2);
        this->deleteItemsFromGroup(group_3);


        int width = this->width() - BORDER;      // определяем ширину нашего виджета
        int height = this->height() - BORDER;    // определяем высоту нашего виджета

        /* Устанавливаем размер сцены по размеру виджета
         * Первая координата - это левый верхний угол,
         * а Вторая - это правый нижний угол
         * */
        scene->setSceneRect(0,0,width,height);

        /* Приступаем к отрисовке произвольной картинки
         * */
        QPen penBlack(Qt::black); // Задаём чёрную кисть
        QPen penGreen(Qt::green); // Задаём чёрную кисть
        QPen penRed(Qt::red);   // Задаём красную кисть
        QBrush global(Qt::green);
        QFont font_point("Arial",10, QFont::Normal);

        Answer* new_ans = scaling(ans_data, height, width);
        Point center(width/2,height/2);
        double cx = center.x;
        double cy = center.y;

        /* Нарисуем черный 3-угольник
         * */
        group_1->addToGroup(scene->addLine(cx+new_ans->p1.x, cy-new_ans->p1.y, cx+new_ans->p2.x, cy-new_ans->p2.y, penBlack));
        group_1->addToGroup(scene->addLine(cx+new_ans->p1.x, cy-new_ans->p1.y, cx+new_ans->p3.x, cy-new_ans->p3.y, penBlack));
        group_1->addToGroup(scene->addLine(cx+new_ans->p3.x, cy-new_ans->p3.y, cx+new_ans->p2.x, cy-new_ans->p2.y, penBlack));
        Point help = Point(cx+new_ans->p1.x, cy-new_ans->p1.y);
        //cout << "len: " << LenSegment(&help, &center) << endl;
        help = Point(cx+new_ans->p2.x, cy-new_ans->p2.y);
        //cout << "len: " << LenSegment(&help, &center) << endl;
        help = Point(cx+new_ans->p3.x, cy-new_ans->p3.y);
        //cout << "len: " << LenSegment(&help, &center) << endl;
        //cout << "R: " << new_ans->R << endl;
        /* Нарисуем красный
         * */

        group_2->addToGroup(scene->addEllipse(cx - new_ans->R, cy - new_ans->R, 2 * new_ans->R, 2* new_ans->R, penRed));

        QGraphicsTextItem *textItem[ans_data->arr.size()];

        //надписи
        for(int i = 0; i < ans_data->arr.size(); i++) {
            //TODO
            group_3->addToGroup(scene->addEllipse(cx + new_ans->arr[i].x - POINT_SIZE, cy - new_ans->arr[i].y - POINT_SIZE,
                                                  POINT_SIZE * 2, POINT_SIZE * 2, penGreen, global));
            textItem[i] = new QGraphicsTextItem();
            textItem[i]->setFont(font_point);

            QString str = ans_data->arr[i].ToQString();
            //cout << str.toStdString() << endl;
            //group_3->addToGroup(scene->addText(str, font_point));
            textItem[i]->setPos(cx + new_ans->arr[i].x, cy - new_ans->arr[i].y);
            textItem[i]->setPlainText(str);
            group_3->addToGroup(textItem[i]);

        }

    }

}






Answer* MyGraphicView::scaling(Answer *ans, double h, double w) {
    Answer *new_ans = new Answer;
   // double h = this->geometry().height();
   // double w = this->geometry().width();
    double new_R = (min(w, h) - BORDER) / 2;
    double scale = new_R / ans->R;
    new_ans->R = new_R;
    new_ans->center = Point(0, 0, OK);
    new_ans->p1 = Point(ans->p1.x - ans->center.x, ans->p1.y - ans->center.y);
    new_ans->p2 = Point(ans->p2.x - ans->center.x, ans->p2.y - ans->center.y);
    new_ans->p3 = Point(ans->p3.x - ans->center.x, ans->p3.y - ans->center.y);
    for(int i = 0; i < ans->arr.size(); i++) {
        new_ans->arr.push_back(Point(ans->arr[i].x - ans->center.x,
                                     ans->arr[i].y - ans->center.y));
    }
    new_ans->diff = ans->diff;
    new_ans->Scale(scale);
    return new_ans;

}

void MyGraphicView::Paint(Answer *ans) {
    ans_data = ans;
    /*cout << ans->p1.x << " " << ans->p1.y << endl;
    cout << ans->p2.x << " " << ans->p2.y << endl;
    cout << ans->p3.x << " " << ans->p3.y << endl;
    cout << ans->diff << endl;
    cout << ans->center.x << " " << ans->center.y << endl;
    cout << ans->R << endl;*/
    slotAlarmTimer();




/*    paint->begin(this); //захват контекста
    Answer* new_ans = scaling(ans);
    QPoint center(width()/2,height()/2);
    paint->setPen(QPen(Qt::red,Qt::SolidLine)); //создать и установить перо - красная сплошная линия
    paint->drawEllipse(center,ans->R,ans->R); //нарисовать окружность по центру
    /*paint.setPen(QPen(Qt::black,Qt::SolidLine)); //создать и установить перо - красная сплошная линия
    double cx = center.x();
    double cy = center.y();
    paint.drawLine(cx+new_ans->p1.x, cy-new_ans->p1.y, cx+new_ans->p2.x, cy-new_ans->p2.y);
    paint.drawLine(cx+new_ans->p1.x, cy-new_ans->p1.y, cx+new_ans->p3.x, cy-new_ans->p3.y);
    paint.drawLine(cx+new_ans->p3.x, cy-new_ans->p3.y, cx+new_ans->p2.x, cy-new_ans->p2.y);

paint->end();//освобождение контекста*/

}

MyGraphicView::~MyGraphicView()
{
    delete ui;
}
/* Метод для удаления всех элементов из группы
 * */
void MyGraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    /* Перебираем все элементы сцены, и если они принадлежат группе,
     * переданной в метод, то удаляем их
     * */
    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}
