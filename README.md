# Overview
This is a simple simulation of a light ray going through a material with a varying refractive index.<br /><br />
![A light path](example.png)<br />
$N(x,y,\lambda) = N_0 + \sin x \sin y \sin A*\lambda$<br />
$N$: Refractive index<br />
$N_0$: Original material refractive index<br />
$A$: Proportionality constant<br />
$\lambda$: Wavelength<br />
# The Physics
I simply used the Snell-Descartes law on refraction. <br />
$N_0 \sin \theta_0 = N_1 \sin \theta_1$ <br />
