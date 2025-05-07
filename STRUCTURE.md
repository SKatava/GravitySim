## 1. Core (Osnovni sustav)

    Application – Pokretanje aplikacije, petlja za renderiranje i događaje.
    Time – Upravljanje vremenom i dt (delta time).
    Logger – Debugging i zapisivanje poruka.

## 2. Rendering

    Renderer – Abstraktni sloj za OpenGL funkcije.
    Shaders – Upravljanje shaderima.
    Textures – Učitavanje i upravljanje teksturama.
    Camera – Pogled na scenu (orbit, free-look).

## 3. Physics

    GravityEngine – Računanje gravitacijskih sila između objekata.
    Integrator – Numerička metoda za ažuriranje položaja tijela.
    Collision – Ako želiš sudare (npr. elastic collisions).

## 4. Entities (Objekti u sceni)

    Entity – Baza za objekte.
    Planet, Star, Asteroid – Specifični entiteti s različitim svojstvima.
    Component System – Ako koristiš komponentni pristup.

## 5. UI (ImGui)

    Scene Hierarchy – Popis entiteta u sceni.
    Inspector – Pregled i uređivanje svojstava objekata.
    Viewport – Prikaz OpenGL rendera u okviru UI-ja.
    Toolbar & Menus – Alati za interakciju.

## 6. Input & Events

    Input Manager – Tipkovnica, miš, gamepad.
    Event System – Komunikacija između modula.

## 7. Serialization (Učitavanje/spremanje scena)

    Scene Loader – Spremanje i učitavanje simulacija (JSON, XML, custom format).
