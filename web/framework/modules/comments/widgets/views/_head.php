
<div id="comments-filter" class="row">
    <div class="col-md-3 col-lg-3">
        <span rel="0">Положительные (<?= $ctype[0]; ?>)</span>
    </div>
    <div class="col-md-3 col-lg-3">
        <span rel="1">Отрицательные (<?= $ctype[1] ?>)</span>
    </div>
    <div class="col-md-3 col-lg-3">
        <span rel="2">Нейтральные (<?= $ctype[2] ?>)</span>
    </div>
    <div class="col-md-3 col-lg-3">
        <span class="active" rel="All">Все (<?= array_sum($ctype) ?>)</span>
    </div>
</div>

