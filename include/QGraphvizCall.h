#ifndef QGRAPHVIZCALL_H
#define QGRAPHVIZCALL_H

#include <QDebug>
#include <QGraphicsView>
#include <QProcess>
#include <QFile>
#include <QtSvg/QGraphicsSvgItem>

namespace QGraphvizCall{

static bool createSvg(const QString& source){
    QFile dot_file("temp.dot");
    if(!dot_file.open(QFile::WriteOnly)){
        qDebug() << "Failed to open \"temp.dot\" in showDot()";
        return false;
    }
    dot_file.write(source.toUtf8());
    dot_file.close();

    QProcess process;
    process.start("dot -Tsvg temp.dot -o temp.svg");
    process.waitForFinished();
    QFile::remove("temp.dot");
    if(!QFile::exists("temp.svg")){
        qDebug() << "QGraphvizCaller: Bad DOT code:" << process.readAllStandardError();
        return false;
    }
    process.close();

    return true;
}

inline QGraphicsScene* getGraphvizScene(const QString& source){
    if(!createSvg(source)) return nullptr;

    QGraphicsSvgItem* item = new QGraphicsSvgItem("temp.svg");
    QFile::remove("temp.svg");

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(item);

    return scene;
}

inline QGraphicsView* show(const QString& source){
    if(QGraphicsScene* scene = getGraphvizScene(source)){
        QGraphicsView* view = new QGraphicsView(scene);
        view->scale(2,2);
        view->setMinimumSize(QSize(300,200));
        view->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);

        view->setScene(scene);
        view->setWindowTitle("Graphviz Output");
        view->show();
		
		return view;
    }
	
	return nullptr;
}

}

#endif
