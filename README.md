# metopt
Реализация алгоритма предсказания оценок пользователями фильма, исследуя имеющиеся оценки с использованием алгоритма SVD(single value decomposition)

#Идеи улучшения
Исследовать пользователей, которые не только оценивают фильмы, но и оставляют рецензии. Возможно, это сделает результаты рекомендаций более точными. 

Исследовать зависимость результата от выбора $\lambda$ - параметра регуляризации.

Добавить оценку ожидаемых фильмов, полученную сравнением их описания, исследовать вероятность просмотра и зависимость оценки от их рекомендации пользователю.

Использовать больше данных и распараллелить алгоритм.

Запустить скрипт на сервере Google App Engine и получать постоянно обновляющуюся статистику оценок пользователя.

Некоторые фильмы были просмотрены пользователем, но их оценка не отображается, так как "Кинопоиск" предоставляет возможность ставить пометку о просмотре. Исследовать предполагаемые оценки.
