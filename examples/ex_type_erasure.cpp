#include <iostream>
#include <Mahi/Util/Templates/TypeErasure.hpp>

using namespace mahi::util;

struct WidgetSpec {
    struct Concept {
        virtual ~Concept() = default;
        virtual void render() const = 0;
    };

    template <class Holder>
    struct Model : public Holder, public virtual Concept {
        using Holder::Holder;
        void render() const {
            model_get(this).render();
        }
    };

    template <class Container>
    struct ExternalInterface : public Container {
        using Container::Container;
        void render() const {
            interface_get(this).render();
        }
    };
};

struct GeometrySpec {
    struct Concept {
        virtual ~Concept() = default;
        virtual int size() const = 0;
    };

    template <class Holder>
    struct Model : public Holder, public virtual Concept {
        using Holder::Holder;
        int size() const {
            return model_get(this).size();
        }
    };

    template <class Container>
    struct ExternalInterface : public Container {
        using Container::Container;
        int size() const {
            return interface_get(this).size();
        }
    };
};

using Widget = TypeErasure<WidgetSpec>;
using Geometry = TypeErasure<GeometrySpec>;

// You can directly combine two concepts:
using GeomWidget = MergeConcepts<Geometry, Widget>;
// You could also create the type-erasure from a list of specs.
// using GeomWidget = TypeErasure<GeometrySpec, WidgetSpec>;

void render_widget(const Widget &w) {
    w.render();
}
void measure_geometry(const Geometry &g) {
    std::cout << "size: " << g.size() << "\n";
}

// MSVC BUG: error C3881: can only inherit constructor from direct base
// void measure_and_render(const GeomWidget &gw) {
//     std::cout << "size: " << gw.size() << "\n";
//     gw.render();
// }

// The above function can now be used for an arbitrary object, provided it
// fulfills the interface.
struct UserWidget {
    void render() const {
        std::cout << "UserWidget::render\n";
    }
    int size() const {
        return 42;
    }
};

int main() {
    UserWidget w;
    render_widget(w);
    std::cout << "\n";
    measure_geometry(w);
    std::cout << "\n";
    // measure_and_render(w);
}
