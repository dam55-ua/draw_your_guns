# Memoria Entregable 1: Makefiles y ccache

**Asignatura:** Desarrollo Colaborativo de Aplicaciones 
**Autores:** Daniel Alarcon Mena - Harjot Kaur - Jonathan Villacís Angamarca
**Fecha:** `por ver`

---

## 1. Descripción del Proyecto y Estructura de Makefiles

* Un único Makefile en la raíz descubre las fuentes con find, replica su estructura en build/, genera los -I y las dependencias automáticamente, por lo que añadir módulos no exige tocarlo.

---

## 2. Análisis de Rendimiento y Experimentos

### 2.1. Compilación Multihilo (`-jN`)

**Pruebas de tiempos de ejecución según el valor de $N$:**

| Valor de $N$ | Tiempo Total (s) | Observaciones |
| :---: | :---: | :--- |
| `-j1` | 3,762 s | Compilación secuencial, CPU ~98% de eficiencia |
| `-j2` | 2,197 s | 1,71× más rápido|
| `-j4` | 1,515 s |  Un 31 % menos de tiempo que -j2; mejor fila en coste/beneficio. |
| `-j8` | 1,207 s | 3,12× (39,0 %). Solo un 20 % mejor que -j4 |
| `-j16` | 1,11 s | Solo un 8 % mejor que -j8; make no lanza más de 12 g++ (12 TUs) y el enlazado es serial |

**Conclusiones:**
* **¿A partir de qué valor de $N$ no se aprecia mejora sustancial?**  
  * A partir de N = 8,  aproximadamente, el incremento no es sustancial. De -j1 a -j4 se reduce un 60 % del tiempo, mientras que de -j8 a -j16 solo un 8 %*

---

### 2.2. Detección y Solución de Fallos en Compilación Paralela

* **¿Se han producido fallos durante la ejecución en paralelo?**  
  * No se han observado fallos durante la ejecución de los diferentes make, debido a que nuestro makefile se encarga de eliminar las posibles condiciones de carrera usando `@mkdir -p $(dir $@)`.

---

### 2.3. Integración y Comparativa de `ccache`

**Comparativa de tiempos de compilación:**

Usamos -j8 para estas mediciones

| Configuración | Tiempo (s) |
| :--- | :---: |
| Compilación limpia sin `ccache` | 1,726 s |
| Compilación limpia con `ccache` (primera ejecución) | 2,151 s |
| Recompilación limpia con `ccache` (segunda ejecución) | 0,405 s|

**Análisis:**  
* Se puede observar que en la primera ejecución de ccache, el tiempo es ligeramente mayor que al hacerlo sin él, ya que se está guardando ese caché que hace que la segunda ejecución de ccache sea mucho más rápida que la primera.

---

### 2.4. Análisis de Estadísticas de `ccache`

**Comandos ejecutados para la prueba:**
```bash
ccache -z        # Limpieza de estadísticas
ccache -C        # Limpieza de la caché
make clean && make -j8  # Primera ejecución (Caché vacía)
make clean && make -j8  # Segunda ejecución (Caché caliente)
```

#### Resultados de `ccache -s`:

* **Primera ejecución (Caché fría):**
  ```text
  Cacheable calls:     12 /  13 (92.31%)
    Hits:               0 /  12 ( 0.00%)
      Direct:           0
      Preprocessed:     0
    Misses:            12 /  12 (100.0%)
  Uncacheable calls:    1 /  13 ( 7.69%)
  Local storage:
    Cache size (GiB): 0.0 / 5.0 ( 0.00%)
    Cleanups:          22
    Hits:               0 /  12 ( 0.00%)
    Misses:            12 /  12 (100.0%)
  ```

* **Segunda ejecución (Caché caliente):**
  ```text
  Cacheable calls:     24 /  26 (92.31%)
    Hits:              12 /  24 (50.00%)
      Direct:          12 /  12 (100.0%)
      Preprocessed:     0 /  12 ( 0.00%)
    Misses:            12 /  24 (50.00%)
  Uncacheable calls:    2 /  26 ( 7.69%)
  Local storage:
    Cache size (GiB): 0.0 / 5.0 ( 0.00%)
    Cleanups:          22
    Hits:              12 /  24 (50.00%)
    Misses:            12 /  24 (50.00%)
  ```

**Análisis de los resultados:**  
* En la primera ejecución observamos 12 misses y en la segunda 12 hits. Esto quiere decir que ninguno de los 12 objetos estaba en la caché y en la segunda se recuperaron directamente de la caché, sin volver a invocar a g++. Además todos los hits fueron directos ya que ni siquiera tuvieron que preprocesarse 