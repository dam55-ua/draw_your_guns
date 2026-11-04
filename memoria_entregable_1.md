# Memoria Entregable 1: Makefiles y ccache

**Asignatura:** Desarrollo Colaborativo de Aplicaciones 
**Autores:** Daniel Alarcon Mena - Harjot Kaur - Jonathan Villacís Angamarca
**Fecha:** `por ver`

---

## 1. Descripción del Proyecto y Estructura de Makefiles

*Breve resumen sobre la estrategia multidirectorio adoptada para la carpeta `src` y cómo se han estructurado los Makefiles para asegurar que sean lo más genéricos posible.*

---

## 2. Análisis de Rendimiento y Experimentos

### 2.1. Compilación Multihilo (`-jN`)

**Pruebas de tiempos de ejecución según el valor de $N$:**

| Valor de $N$ | Tiempo Total (s) | Observaciones |
| :---: | :---: | :--- |
| `-j1` | -- | Compilación secuencial |
| `-j2` | -- | |
| `-j4` | -- | |
| `-j8` | -- | |
| `-j16` | -- | |

**Conclusiones:**
* **¿A partir de qué valor de $N$ no se aprecia mejora sustancial?**  
  *Escribe aquí tus conclusiones basándote en el número de núcleos físicos/lógicos de la máquina de pruebas y los tiempos obtenidos.*

---

### 2.2. Detección y Solución de Fallos en Compilación Paralela

* **¿Se han producido fallos durante la ejecución en paralelo?**  
  *(Sí / No)*

* **Causa del problema (si aplica):**  
  *Explicación de posibles condiciones de carrera, faltas de dependencias entre reglas o ficheros generados.*

* **Solución aplicada:**  
  *Cómo se ajustaron las reglas en el `Makefile` para garantizar una correcta compilación paralela.*

---

### 2.3. Integración y Comparativa de `ccache`

**Comparativa de tiempos de compilación:**

| Configuración | Tiempo (s) |
| :--- | :---: |
| Compilación limpia sin `ccache` | -- |
| Compilación limpia con `ccache` (primera ejecución) | -- |
| Recompilación limpia con `ccache` (segunda ejecución) | -- |

**Análisis:**  
*Comentario conciso sobre la diferencia de tiempos observada entre compilar sin la herramienta y reutilizar la caché.*

---

### 2.4. Análisis de Estadísticas de `ccache`

**Comandos ejecutados para la prueba:**
```bash
ccache -z        # Limpieza de estadísticas
ccache -C        # Limpieza de la caché
make clean && make -jN  # Primera ejecución (Caché vacía)
make clean && make -jN  # Segunda ejecución (Caché caliente)
```

#### Resultados de `ccache -s`:

* **Primera ejecución (Caché fría):**
  ```text
  [Pegar aquí la salida de `ccache -s` tras la primera compilación]
  ```

* **Segunda ejecución (Caché caliente):**
  ```text
  [Pegar aquí la salida de `ccache -s` tras la segunda compilación]
  ```

**Análisis de los resultados:**  
*Explicación breve de los datos observados (porcentaje de hits/misses, ratio de reutilización de la caché, etc.).*