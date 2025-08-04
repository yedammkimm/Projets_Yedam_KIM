#!/bin/bash

declare -a types=("Électricité" "Déchets" "Eau" "Gaz")

for month in {01..12}; do
    for type in "${types[@]}"; do
        curl -X POST http://127.0.0.1:5000/factures -H "Content-Type: application/json" -d "{\"type_fac\": \"$type\", \"montant\": $((RANDOM % 100 + 50)).0, \"logement_id\": 1, \"date\": \"2024-$month-15 12:00:00\"}"
    done
done
