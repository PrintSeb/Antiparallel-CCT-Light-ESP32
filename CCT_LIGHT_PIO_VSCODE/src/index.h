const char htmlContent[] PROGMEM = R"=====(
<!DOCTYPE html><html>
<head> 
<meta charset=/"UTF-8/"> 
<meta name=/"viewport/" content=/"width=device-width, initial-scale=1/"> 
<title>Hue Light</title> 
<link rel=/"stylesheet/" href=/"https://diyhue.org/cdn/bootstrap.min.css/"> 
<link rel=/"stylesheet/" href=/"https://diyhue.org/cdn/ion.rangeSlider.min.css/"/> 
<script src=/"https://diyhue.org/cdn/jquery-3.3.1.min.js/"></script> 
<script src=/"https://diyhue.org/cdn/bootstrap.min.js/"></script> 
<script src=/"https://diyhue.org/cdn/ion.rangeSlider.min.js/"></script> 
</head> 
<body> 
<nav class=/"navbar navbar-expand-lg navbar-light bg-light rounded/"> 
<button class=/"navbar-toggler/" type=/"button/" data-toggle=/"collapse/" data-target=/"#navbarToggler/" aria-controls=/"navbarToggler/" aria-expanded=/"false/" aria-label=/"Toggle navigation/"> 
<span class=/"navbar-toggler-icon/"></span> 
</button> 
<h2></h2> 
<div class=/"collapse navbar-collapse justify-content-md-center/" id=/"navbarToggler/"> 
<ul class=/"nav nav-pills/"> 
<li class=/"nav-item/"> 
<a class=/"nav-link active/" data-toggle=/"pill/" href=/"#home/">Home</a> 
</li><li class=/"nav-item/"> 
<a class=/"nav-link/" data-toggle=/"pill/" href=/"#menu1/">Settings</a> 
</li><li class=/"nav-item/"> 
<a class=/"nav-link/" data-toggle=/"pill/" href=/"#menu2/">Network</a> 
</li><li class=/"nav-item/"> 
<a class=/"nav-link/" data-toggle=/"pill/" href=/"#/" disabled> 
</a> 
</li><li class=/"nav-item/"> 
<a class=/"nav-link/" data-toggle=/"pill/" href=/"#/" disabled> 
</a> 
</li></ul> 
</div></nav> 
<div class=/"tab-content/"> 
<div class=/"tab-pane container active/" id=/"home/"> 
<br><br><form> 
<div class=/"form-group row/"> 
<label for=/"power/" class=/"col-sm-2 col-form-label/">Power</label> 
<div class=/"col-sm-10/"> 
<div id=/"power/" class=/"btn-group/" role=/"group/"> 
<button type=/"button/" class=/"btn btn-default border/" id=/"power-on/">On</button> 
<button type=/"button/" class=/"btn btn-default border/" id=/"power-off/">Off</button> 
</div></div></div><div class=/"form-group row/"> 
<label for=/"bri/" class=/"col-sm-2 col-form-label/">Brightness</label> 
<div class=/"col-sm-10/"> 
<input type=/"text/" id=/"bri/" class=/"js-range-slider/" name=/"bri/" value=/"/"/> 
</div></div><div class=/"form-group row/"> 
<label for=/"hue/" class=/"col-sm-2 col-form-label/">Color</label> 
<div class=/"col-sm-10/"> 
<canvas id=/"hue/" width=/"320px/" height=/"320px/" style=/"display:none;/"></canvas> 
</div></div><div class=/"form-group row/"> 
<label for=/"color/" class=/"col-sm-2 col-form-label/">Color Temp</label> 
<div class=/"col-sm-10/"> 
<canvas id=/"ct/" width=/"320px/" height=/"50px/" style=/"border:1px solid #d3d3d3;/"></canvas> 
</div></div></form> 
</div><div class=/"tab-pane container fade/" id=/"menu1/"> 
<br><form method=/"POST/" action=/"//"> 
<div class=/"form-group row/"> 
<label for=/"name/" class=/"col-sm-2 col-form-label/">Light Name</label> 
<div class=/"col-sm-6/"> 
<input type=/"text/" class=/"form-control/" id=/"name/" name=/"name/"> 
</div></div><div class=/"form-group row/"> 
<label class=/"control-label col-sm-2/" for=/"startup/">Default Power:</label> 
<div class=/"col-sm-4/"> 
<select class=/"form-control/" name=/"startup/" id=/"startup/"> 
<option value=/"0/">Last State</option> 
<option value=/"1/">On</option> 
<option value=/"2/">Off</option> 
</select> 
</div></div><div class=/"form-group row/"> 
<label class=/"control-label col-sm-2/" for=/"scene/">Default Scene:</label> 
<div class=/"col-sm-4/"> 
<select class=/"form-control/" name=/"scene/" id=/"scene/"> 
< <option value=/"0/">Relax</option> 
<option value=/"1/">Read</option> 
<option value=/"2/">Concentrate</option> 
<option value=/"3/">Energize</option> 
<option value=/"4/">Bright</option> 
<option value=/"5/">Dimmed</option> 
</select> 
</div></div><div class=/"form-group row/"> 
<label for=/"colorstate/" class=/"col-sm-2 col-form-label/">CCW Pin</label> 
<div class=/"col-sm-3/"> 
<input type=/"number/" class=/"form-control/" id=/"colorstate/" name=/"colorstate/" placeholder=/"/"> 
</div></div><div class=/"form-group row/"> 
<label for=/"brightstate/" class=/"col-sm-2 col-form-label/">Brightness Pin</label> 
<div class=/"col-sm-3/"> 
<input type=/"number/" class=/"form-control/" id=/"brightstate/" name=/"brightstate/" placeholder=/"/"> 
</div></div><div class=/"form-group row/"> 
<label class=/"control-label col-sm-2/" </div></div><div class=/"form-group row/"> 
<div class=/"col-sm-10/"> 
<button type=/"submit/" class=/"btn btn-primary/">Save</button> 
</div></div></form> 
</div><div class=/"tab-pane container fade/" id=/"menu2/"> 
<br><form method=/"POST/" action=/"//"> 
<div class=/"form-group row/"> 
<label class=/"control-label col-sm-2/" for=/"dhcp/">DHCP:</label> 
<div class=/"col-sm-3/"> 
<select class=/"form-control/" name=/"dhcp/" id=/"dhcp/"> 
<option value=/"1/">On</option> 
<option value=/"0/">Off</option> 
</select> 
</div></div><div class=/"form-group row/"> 
<label for=/"addr/" class=/"col-sm-2 col-form-label/">Ip</label> 
<div class=/"col-sm-4/"> 
<input type=/"text/" class=/"form-control/" id=/"addr/" name=/"addr/"> 
</div></div><div class=/"form-group row/"> 
<label for=/"sm/" class=/"col-sm-2 col-form-label/">Submask</label> 
<div class=/"col-sm-4/"> 
<input type=/"text/" class=/"form-control/" id=/"sm/" name=/"sm/"> 
</div></div><div class=/"form-group row/"> 
<label for=/"gw/" class=/"col-sm-2 col-form-label/">Gateway</label> 
<div class=/"col-sm-4/"> 
<input type=/"text/" class=/"form-control/" id=/"gw/" name=/"gw/"> 
</div></div><div class=/"form-group row/"> 
<div class=/"col-sm-10/"> 
<button type=/"submit/" class=/"btn btn-primary/">Save</button> 
</div></div></form> 
</div></div><script src=/"https://diyhue.org/cdn/color.min.js/"></script> 
</body></html>
)=====";