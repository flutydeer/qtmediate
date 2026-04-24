#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <QComboBox>

#include <QMWidgets/qmwidgetsglobal.h>

class QM_WIDGETS_EXPORT CComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY(CornerPreference cornerPreference READ cornerPreference WRITE setCornerPreference)

public:
    enum CornerPreference { Default = 0, DoNotRound = 1, Round = 2, RoundSmall = 3 };
    Q_ENUM(CornerPreference)

    explicit CComboBox(QWidget *parent = nullptr);
    ~CComboBox();

    bool enableWheel() const;
    void setEnableWheel(bool enableWheel);

    static void setDefaultCornerPreference(CornerPreference preference);
    static CornerPreference defaultCornerPreference();

    void setCornerPreference(CornerPreference preference);
    CornerPreference cornerPreference() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void showPopup() override;

private:
    bool m_enableWheel;
    CornerPreference m_cornerPreference;
};

#endif // CCOMBOBOX_H
