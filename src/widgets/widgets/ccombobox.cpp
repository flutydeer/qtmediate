#include "ccombobox.h"

#include <QGuiApplication>
#include <QListView>
#include <QStyleHints>

#ifdef Q_OS_WINDOWS
#  include <dwmapi.h>
#endif

static CComboBox::CornerPreference g_defaultCornerPreference = CComboBox::RoundSmall;

CComboBox::CComboBox(QWidget *parent)
    : QComboBox(parent), m_enableWheel(false), m_cornerPreference(g_defaultCornerPreference) {
    setView(new QListView());

#ifdef Q_OS_WINDOWS
    if (auto *popup = view()->parentWidget()) {
        popup->setWindowFlag(Qt::NoDropShadowWindowHint, true);
    }
#endif
}

CComboBox::~CComboBox() {
}

bool CComboBox::enableWheel() const {
    return m_enableWheel;
}

void CComboBox::setEnableWheel(bool enableWheel) {
    m_enableWheel = enableWheel;
}

void CComboBox::setDefaultCornerPreference(CornerPreference preference) {
    g_defaultCornerPreference = preference;
}

CComboBox::CornerPreference CComboBox::defaultCornerPreference() {
    return g_defaultCornerPreference;
}

void CComboBox::setCornerPreference(CornerPreference preference) {
    m_cornerPreference = preference;
}

CComboBox::CornerPreference CComboBox::cornerPreference() const {
    return m_cornerPreference;
}

void CComboBox::wheelEvent(QWheelEvent *event) {
    if (m_enableWheel) {
        QComboBox::wheelEvent(event);
    }
}

void CComboBox::showPopup() {
    QComboBox::showPopup();

#ifdef Q_OS_WINDOWS
    QWidget *popup = view() ? view()->window() : nullptr;
    if (!popup || !popup->winId())
        return;

    constexpr int mgn = 1;
    constexpr int DWMWA_USE_IMMERSIVE_DARK_MODE_ = 20;
    constexpr int DWMWA_WINDOW_CORNER_PREFERENCE_ = 33;
    constexpr int DWMWA_BORDER_COLOR_ = 34;
    constexpr COLORREF DWMWA_COLOR_DEFAULT_ = 0xFFFFFFFF;
    constexpr COLORREF DWMWA_COLOR_NONE_ = 0xFFFFFFFE;
    DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
    INT dwcp = m_cornerPreference;
#  if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    const UINT dark = QGuiApplication::styleHints()->colorScheme() != Qt::ColorScheme::Light;
#  else
    const UINT dark = 1;
#  endif
    MARGINS margins = {mgn, mgn, mgn, mgn};

    HWND hwnd = reinterpret_cast<HWND>(popup->winId());
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE_, &dark, sizeof(dark));
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE_, &dwcp, sizeof(dwcp));
    // Update the border color for the current scheme: suppress the DWM border in light mode,
    // keep the default one in dark mode. Do NOT force a synchronous non-client refresh here
    // (SWP_FRAMECHANGED + RDW_UPDATENOW): those synchronous cross-process calls can flood
    // dwm.exe and freeze the desktop compositor.
    const COLORREF borderColor = dark ? DWMWA_COLOR_DEFAULT_ : DWMWA_COLOR_NONE_;
    DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR_, &borderColor, sizeof(borderColor));
#endif
}
