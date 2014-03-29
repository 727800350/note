%% MetaPost Macros

%% prologues:=3;
%% outputtemplate:=("%j" & ".mps");
%% verbatimtex
%% %&latex
%% \documentclass{minimal}
%% \begin{document}
%% etex

%% scale rule
numeric u; u := 1cm;

numeric pi; pi := 3.1415926;
numeric radian; radian := 180/pi;

%% trigometric functions
%% sind and cosd expect their argument in degress, not in radians
vardef sin primary x = (sind(x*radian)) enddef;
vardef cos primary x = (cosd(x*radian)) enddef;
vardef tan primary x = (sin(x)/cos(x)) enddef;
vardef cot primary x = (cos(x)/sin(x)) enddef;
vardef tand primary x = (sind(x)/cosd(x)) enddef;
vardef cotd primary x = (cosd(x)/sind(x)) enddef;
% hyperbolic functions
vardef sinh primary x = save xx ; xx = exp xx ; (xx-1/xx)/2 enddef ;
vardef cosh primary x = save xx ; xx = exp xx ; (xx+1/xx)/2 enddef ;
vardef tanh primary x = (sinh(x)/cosh(x)) enddef;
vardef coth primary x = (cosh(x)/sinh(x)) enddef;
% inverse trigonometric and hyperbolic functions
vardef arcsind primary x = angle((1+-+x,x)) enddef;
vardef arccosd primary x = angle((x,1+-+x)) enddef;
vardef arcsin primary x = ((arcsind(x))/radian) enddef;
vardef arccos primary x = ((arccosd(x))/radian) enddef;
vardef arccosh primary x = ln(x+(x+-+1)) enddef;
vardef arcsinh primary x = ln(x+(x++1)) enddef;

vardef sqr primary x = (x*x) enddef;
vardef log primary x = (if x=0: 0 else: mlog(x)/mlog(10) fi) enddef;
vardef ln primary x = (if x=0: 0 else: mlog(x)/256 fi) enddef;
vardef exp primary x = ((mexp 256)**x) enddef;
vardef inv primary x = (if x=0: 0 else: x**-1 fi) enddef;
vardef pow (expr x,p) = (x**p) enddef;

%% projection of m on path a--b
vardef projection(expr m,a,b) =
    save h; pair h;
    h - m = whatever * (b-a) rotated 90;
    h = whatever [a,b];
    h
enddef;

%% the middle point of a line
vardef iso(expr a,b) = 
	save c; pair c;
	c = .5[a,b];
	c
enddef;

%% the returned point is besides the counterclockwise direction of [a,b]
vardef mediatrice(expr a,b) = 
	save c; pair c;
	c := b rotatedaround(iso(a,b),90);
	c
enddef;

%% get the symmetry point of s about [a,b] 
vardef symtrie(expr s,a,b) = 
	save c; pair c;
	c := s rotatedaround(projection(s,a,b),180);
	c
enddef;

%% get the bisector point in [a,b]
vardef bisector(expr a,b,common) = 
	save c; pair c;
	c = whatever[a,b];
	c - common = whatever*dir(1/2*angle(a-common) + 1/2*angle(b-common));
	c
enddef;

% set some user-adjustable constants
angle_radius := .4u;
angle_delta := .05u;
mark_size := .2u;
def mark_angle(expr A, common, B, n) =
	% draw 1, 2, 3 or 4 arcs
	draw_angle(A, common, B, angle_radius);
	if n>1: draw_angle(A, common, B,angle_radius+angle_delta); fi;
	if n>2: draw_angle(A, common, B,angle_radius-angle_delta); fi;
	if n>3: draw_angle(A, common, B,angle_radius+2*angle_delta); fi;
enddef;

def draw_angle(expr endofa, common, endofb, r) =
	begingroup
	save tn; tn := turningnumber(common--endofa--endofb--cycle);
	draw (unitvector(endofa-common){(endofa-common) rotated(tn*90)} .. unitvector(endofb-common)) scaled r shifted common;
	endgroup
	enddef;

def mark_right_angle(expr endofa, common, endofb) =
	begingroup
	save tn; tn := turningnumber(common--endofa--endofb--cycle);
	draw ((1,0)--(1,1)--(0,1)) zscaled(mark_size* unitvector((1+tn)*endofa+(1-tn)*endofb-2*common)) shifted common;
	endgroup
enddef;

%% make n marks in the middle of expr p
%% eg: tick(A--D,2), make 2 marks in the middle of path A--D
def tick(expr p, n) =
	begingroup
	save midpnt; midpnt = 0.5*arclength(p);
	% find the time when half-way the path
	for i=-(n-1)/2 upto (n-1)/2:
		draw_mark(p, midpnt+mark_size*i/2);
		% place n tick marks
	endfor;
	endgroup
enddef;

def draw_mark(expr p, m) =
	begingroup
	save t, dm; pair dm;
	t = arctime m of p;
	% find a vector orthogonal to p at time t
	dm = mark_size*unitvector(direction t of p rotated 90);
	draw(-1/2dm..1/2dm) shifted (point t of p);
	% draw tick mark
	endgroup
enddef;

%% use drawpoint p or drawpoint(p)
def draw_point primary p = 
	draw p withpen pencircle scaled 3;
enddef;

vardef nomme@#(suffix p) = 
	draw_point p;
	label.@#(str p,p);
enddef;
