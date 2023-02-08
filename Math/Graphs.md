1. **Неориентированный граф**
    - Определение
        * Неорграф - граф, в котором движение между вершинами может проиводиться в две стороны.
    - Условные обозначения
        * G - Граф
        * V - Вершина
        * R - Ребро
    - Понятие петли и мультиребра
        * Петля - ребро, инцидентное (ребро прилегает к вершине) одной и той же вершине
        * Мультиребро - множество ребер, имеющих одинаковые инцидентные вершины (???)
    - Степень врешины
        * Число ребер инцидентных данной вершине
    - Первая теорема графов
        * Сумма степенней вершин графа всегда четная и равна R * 2
2. **Регулярный граф**
    - Граф, степени вершин которого равны
3. **Матрица инцидентности**
    - Матрица, отображающая вход выход ребер в вершины
    - Пример (строки — рёбра (1-6), столбцы — вершины (e1-e7)):
    
    | Граф | Матрица инцидентности |
    | ---  | --- |
    | ![](https://user-images.githubusercontent.com/43097289/98879664-20917f80-2497-11eb-8225-cbb64f5c9264.png) | ![](https://user-images.githubusercontent.com/43097289/98879836-7cf49f00-2497-11eb-819f-ee85e1fab9da.png) |

    В случае ориентированного графа может также использоваться -1
4. **Мультиграф**
    - Граф, в котором разрешается присутсвие кратных (имеющих одинаковые инцидентные вершины) рёбер
5. **Полный граф**
    - Каждая пара вершин смежна (все соединены со всеми)
6. **Двудольный граф**
    - Граф называется двудольным, если его вершины можно разбить на два множества так, что у каждого ребра конечные вершины принадлежат разным множествам.
7. **Ориентированный граф**
    - Граф, в котором ребра имеют направленность
8. **Равенство о суммах входящих и выходящих вершин орграфа**
    - Сумма входящих и выходящих вершин орграфа равна удвоенному количеству рёбер
9. **Смежные вершины в (не)ориентированных графах**
    - Две вершины смежны, если являются разными концами одного ребра
10. **Матрица смежности**
    - Матрица смежности графа G с конечным числом вершин n (пронумерованных числами от 1 до n) — это квадратная матрица A размера n, в которой значение элемента a<sub>ij</sub> равно числу рёбер из i-й вершины графа в j-ю вершину.
    - Пример:
    
    | Граф | Матрица смежности
    | - | - |
    | ![](https://user-images.githubusercontent.com/43097289/98881301-b975ca00-249a-11eb-9311-17dcbb18d9f0.png) | ![](https://user-images.githubusercontent.com/43097289/98882190-cc899980-249c-11eb-87a1-5db4cae385f9.png) |11. **Маршруты, пути и циклы в графе.**
    - Маршрут - Последовательность вершин, начинающаяся и заканчивающимся определеннымим вершинами
    - Путь - Последовательность вершин, без повторений, в которой две соседние вершины смежны.
    - Цикл - Замкнутый маршрут
12. **Простые и составные циклы**
    - Цикл простой, если не прохоит ни через одну из вершин графа более 1 раза.
    - Составной, если проходит через вершину более одного раза
13. **Обхват графа**
    - Длина наименьшего цикла, содержащегося в графе
14. **Связные графы**
    - Компоненты связывания
        * Граф называется связанным, если любые две его вершины можно соединить путем или маршрутом
15. **Расстояние между двумя связными вершинами**
    - Диаметр графа
        * Максимальное расстояние между двумя его вершинами.
    - Эксцентриситет вершины
        * Расстояние от этой вершины до наиболее удаленных от нее вершин.
    - Радиус графа
        * Максимально возможное расстояние между двумя его вершинами
16. **Понятие сильной и слабой связи орграфа**
    - Орграф сильно связный, или просто сильный, если все его вершины взаимно достижимы.
    - Односторонне связный, или просто односторонний, если для любых двух вершин, по крайней мере, одна достижима из другой.
    - Слабо связный, или просто слабый, если при игнорировании направления дуг получается связный (мульти)граф.

17. **Эйлеров цикл**
    - Эйлеровым путем в графе называется путь, который проходит по каждому ребру, причем ровно один раз.
    - Эйлеров цикл — замкнутый эйлеров путь.
    - Для того, чтобы граф был эйлеровым, необходимо чтобы:
        1. Все вершины имели четную степень.
        2. Граф был связным или будет таковым, если удалить из него все изолированные вершины.
    - Эйлерова цепь — см. путь.
    - Эйлеров граф — граф, содержащий эйлеров цикл.
    - Полуэйлеров граф — граф, содержащий эйлеров путь.
18. **Теоремы, устанавливающие является ли граф Эйлеровым**
    - Ориентированный граф **G=(V, A)** содержит эйлеров цикл тогда и только тогда, когда он сильно связан или среди его компонент сильной связности только одна содержит ребра (а все остальные являются изолированными вершинами) и для каждой вершины графа её входящая степень равна её исходящей степени, то есть в вершину входит столько же ребер, сколько из неё и выходит.
19. **Гамильтонов цикл**
    - Гамильтонов путь — путь, который является простым путём (путём без петель), проходящим через каждую вершину графа ровно один раз.
    - Цепь — см. путь.
    - Гамильтонов цикл — это такой цикл (замкнутый путь), который проходит через каждую вершину данного графа ровно по одному разу.
    - Гамильто́нов граф — граф, содержащий гамильтонов цикл.
    - Полугамильтоновым графом называется граф, содержащий простую цепь, проходящую через каждую его вершину ровно один раз. При этом, всякий гамильтонов граф является полугамильтоновым. 
20. **Теорема Кенига**
    - Вершинное покрытие графа — это множество вершин, такое, что любое ребро графа имеет хотя бы одну конечную вершину из этого множества. Вершинное покрытие называется наименьшим, если никакое другое вершинное покрытие не имеет меньшего числа вершин.
    - Паросочетанием в графе называется множество рёбер, не имеющих общих конечных вершин. Паросочетание называется наибольшим, если никакое другое паросочетание не содержит большего числа рёбер.
    - Теорема:
        * В любом двудольном графе число рёбер в наибольшем паросочетании равно числу вершин в наименьшем вершинном покрытии.