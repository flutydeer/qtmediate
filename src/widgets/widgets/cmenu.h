#ifndef CMENU_H
#define CMENU_H

#include <QMenu>

#include <QMWidgets/qmwidgetsglobal.h>

class CMenuPrivate;

class QM_WIDGETS_EXPORT CMenu : public QMenu {
    Q_OBJECT
    Q_PROPERTY(CornerPreference cornerPreference READ cornerPreference WRITE setCornerPreference)

public:
    enum CornerPreference { Default = 0, DoNotRound = 1, Round = 2, RoundSmall = 3 };
    Q_ENUM(CornerPreference)

    explicit CMenu(QWidget *parent = nullptr);
    explicit CMenu(const QString &title, QWidget *parent = nullptr);
    ~CMenu();

    void setCornerPreference(CornerPreference preference);
    CornerPreference cornerPreference() const;

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const;

private:
    CMenuPrivate *d;
};

#endif // CMENU_H
