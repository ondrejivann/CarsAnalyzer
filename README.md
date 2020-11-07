# CarsAnalyzer
Virtual Reality Laboratory - OpenCV Project

## Trackování aut a jejich počet
Z pohybujících se kontur vytvořím skvrny, které reprezentují auta. V každém snímku se snažím navazovat na sebe skvrny tak, aby se jednalo o tu samou (v jednoduchosti se snažím najít nejbližší) z předchozího kroku. Tak vím, že se jedná o jedno auto. Pokud skvrna přejede přes určitou linii, započítá se do počtu aut v určitém směru.
## Měření rychlosti
Na mapě jsem zhruba změřil dráhu od jedné linie po druhou (35m-40m) a sleduju za jak dlouho auto vzdálenost přejede. Z toho vypočítám průměrnou auta rychlost a poté průměrnou rychlost všech aut. (V jednom směru).
## Trackování bílých aut a jejich počet
Vyříznu si z cameraFramu (cv::Mat) bouding box auta. Z vyříznuté části sleduju bílou barvu. Vytvořím kontury pro bílou barvu a spočítám jejich velikost (area). Poté spočítám velikost kontury auta a poměrem se snažím řict jakou velikost z kontury auta tvoří bílá barva. Bohužel ne všechny auta to bere. První a poslední auto to nezachytí.
