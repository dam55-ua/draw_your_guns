# Documentación del Proyecto - Hito 1: Alpha

**Asignatura:** Desarrollo Colaborativo de Aplicaciones (DCA)  
**Proyecto:** *Draw your guns!*  

---

## 1. Game Design Document (GDD) Básico

### 1.1. Visión General y Concepto del Juego
El proyecto consiste en un juego de estrategia en 2D por turnos inspirado en el clásico *Worms*. Los jugadores controlan personajes en un escenario e intentan derrotar a sus oponentes gestionando su posicionamiento, apuntando con precisión y gestionando sus recursos. 

Un elemento diferenciador clave es el **diseño dinámico de armas mediante un lienzo de dibujo en tiempo real** y eventos de mapa progresivos que aumentan la intensidad a medida que avanza la partida.

---

### 1.2. Mecánicas Básicas
El bucle jugable se articula a través de las siguientes mecánicas fundamentales:

* **Movimiento y Física:**
  * **Moverse por el terreno:** Desplazamiento lateral del personaje a lo largo de la superficie del mapa.
  * **Saltar:** Mecánica de salto para superar obstáculos o posicionarse estratégicamente.
* **Sistema de Combate y Armamento:**
  * **Apuntar con arma:** Ajuste del ángulo y la trayectoria de disparo.
  * **Disparar:** Ejecución del ataque con el arma seleccionada.
  * **Recompensa por impacto:** Los jugadores obtienen *créditos* al acertar impactos exitosos sobre los enemigos.
* **Estructura del Juego:**
  * **Sistema de Rondas/Turnos:** Una *ronda* queda completada cuando todos los jugadores en la partida han consumido su turno correspondiente.

---

### 1.3. Componentes y Sistemas Especiales

#### A. Taller/Editor de Armas Dibujadas
Durante su turno, el jugador puede desplegar un menú emergente con un lienzo en blanco para personalizar y crear sus propias proyectiles o estructuras:
* **Formas geométricas y comportamiento:**
  * **Círculos:** Generan proyectiles de tipo *Granada*.
  * **Rectángulos:** Permiten generar *Paredes* o coberturas estáticas en el terreno.
* **Equilibrio de juego y economía:**
  * **Tamaño limitado:** Existe una dimensión máxima permitida según la categoría de la forma.
  * **Relación Daño/Coste:** A mayor tamaño de la forma dibujada, mayor será el daño infligido.
  * **Coste en Créditos:** Dibujar formas de mayor tamaño consume más *créditos*, los cuales se deben haber acumulado previamente mediante la *Recompensa por impacto*.

#### B. Personalización de Personajes
* Menú de selección previo a la partida para elegir accesorios en la cabeza (gorros, cascos, etc.).
* La base y figura de los personajes se mantiene uniforme para todos los jugadores.

#### C. Eventos Dinámicos de Mapa
Para evitar partidas estancadas y aportar dinamismo, el escenario evoluciona según el número de ronda actual:
* **Ataque Aéreo (Rondas múltiplos de 5):** Caída masiva de proyectiles tipo racimo/misiles desde la parte superior de la pantalla. No cubren la totalidad del mapa, obligando a buscar cobertura.
* **Subida del Nivel del Mar (A partir de la ronda 17):** Un bloque que representa el agua asciende desde el límite inferior del mapa. La velocidad de ascenso se incrementa de forma exponencial o proporcional con cada nueva ronda transcurrida.

---

## 2. Estrategia de Gestión de Ramas (Git)

* **Estrategia seleccionada:** GitHub Flow
* **Justificación:** Se ha seleccionado GitHub Flow por ser un flujo de trabajo ligero, ágil y adecuado para un equipo reducido de 3 personas. Al tener un ciclo de desarrollo iterativo basado en entregas cortas, esta estrategia nos permite mantener una rama `develop` siempre estable y desplegable. Cada nueva funcionalidad o corrección se desarrolla en ramas descriptivas derivadas de `develop` (por ejemplo, `feat/player`), integrándose mediante Pull Requests que requieren la revisión y aprobación de al menos un compañero antes del *merge*.

---

## 3. Sistema de Bugtracking Elegido

* **Herramienta elegida:** GitHub Issues
* **Justificación:** Al alojar el proyecto en GitHub, usar GitHub Issues nos permite gestionar incidencias y tareas en la misma plataforma que el código, sin herramientas externas. Cada *issue* se etiqueta por tipo (bug, mejora, tarea), se asigna a quien lo trabaja y se vincula con el Pull Request que lo resuelve mediante referencias automáticas, manteniendo un historial trazable de cada fallo o funcionalidad.
* **Automatización del triaje:** Pretendemos automatizar el triaje de los *issues* con **Jev** (modelo de decisión de TypeSafe, `typesafe/jev-1.13`) accesible a través de la API de Decisions de **OpenRouter**. A diferencia de un modelo de lenguaje genérico, Jev no genera texto sino que responde a preguntas tipadas (elección, sí/no o puntuación) devolviendo la probabilidad asociada, lo que nos permite clasificar cada *issue* entrante (tipo, urgencia, área asignada) y aplicar las etiquetas correspondientes con un umbral de confianza, integrado en un workflow de GitHub Actions. Las decisiones con confianza por debajo del umbral se descartan y quedan para revisión manual, de modo que la automatización funciona como una señal de apoyo al equipo y no como una autoridad que cierre o reasigne incidencias por sí sola.

---

## 4. Librerías Adicionales

* **Raylib:** Utilizada para la gestión de gráficos, físicas básicas y entrada de usuario.
* **[Otras librerías]:** Para la entrega de este `Hito 1` no se han añadido otras librerias.
