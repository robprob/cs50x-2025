SELECT title
FROM movies m
JOIN stars s
ON s.movie_id = m.id
JOIN people p
ON p.id = s.person_id
WHERE p.name IN ('Bradley Cooper', 'Jennifer Lawrence')
GROUP BY title
HAVING COUNT(title) = 2;
