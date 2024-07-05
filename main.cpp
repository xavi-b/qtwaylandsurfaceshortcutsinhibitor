#include <QApplication>
#include <QPushButton>

#include <QWindow>
#include <qpa/qplatformwindow_p.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-server-core.h>

#include "keyboard-shortcuts-inhibit-unstable-v1-client-protocol.h" // Include the generated header

// Wayland global variables
struct wl_keyboard *keyboard = nullptr;
struct zwp_keyboard_shortcuts_inhibit_manager_v1 *inhibit_manager = nullptr;
struct zwp_keyboard_shortcuts_inhibitor_v1 *inhibitor = nullptr;

// Keyboard shortcuts inhibit handler
void handle_inhibit_shortcuts(void *data, struct zwp_keyboard_shortcuts_inhibitor_v1 *manager)
{
    qDebug() << "Keyboard shortcuts inhibited";
}

void handle_shortcuts_resumed(void *data, struct zwp_keyboard_shortcuts_inhibitor_v1 *manager)
{
    qDebug() << "Keyboard shortcuts resumed";
}

// Inhibit manager listener
static const struct zwp_keyboard_shortcuts_inhibitor_v1_listener inhibitor_listener
    = {handle_inhibit_shortcuts, handle_shortcuts_resumed};

// Registry global handler
void handle_global(void *data,
                   struct wl_registry *registry,
                   uint32_t name,
                   const char *interface,
                   uint32_t version)
{
    if (strcmp(interface, zwp_keyboard_shortcuts_inhibit_manager_v1_interface.name) == 0) {
        inhibit_manager = static_cast<zwp_keyboard_shortcuts_inhibit_manager_v1 *>(
            wl_registry_bind(registry,
                             name,
                             &zwp_keyboard_shortcuts_inhibit_manager_v1_interface,
                             1));
        qDebug() << "zwp_keyboard_shortcuts_inhibit_manager_v1";
    }
}

// Registry listener
struct wl_registry_listener registry_listener = {handle_global, nullptr};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton w;
    w.show();
    QObject::connect(&w, &QPushButton::clicked, [&]() {
        if (inhibitor) {
            zwp_keyboard_shortcuts_inhibitor_v1_destroy(inhibitor);
            inhibitor = nullptr;
        } else {
            auto seat = qApp->nativeInterface<QNativeInterface::QWaylandApplication>()->seat();
            auto surface = w.windowHandle()
                               ->nativeInterface<QNativeInterface::Private::QWaylandWindow>()->surface();
            qDebug() << "surface:" << (size_t) surface;
            inhibitor = zwp_keyboard_shortcuts_inhibit_manager_v1_inhibit_shortcuts(inhibit_manager,
                                                                                    surface,
                                                                                    seat);
            qDebug() << "zwp_keyboard_shortcuts_inhibit_manager_v1_inhibit_shortcuts:"
                     << (size_t) inhibitor;
            int res = zwp_keyboard_shortcuts_inhibitor_v1_add_listener(inhibitor,
                                                                       &inhibitor_listener,
                                                                       nullptr);
            qDebug() << "zwp_keyboard_shortcuts_inhibitor_v1_add_listener:" << res;
        }
    });

    return app.exec();
}
