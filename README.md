# Overview
This is a simple simulation of a light ray going through a material with a varying refractive index.
<p align="center">
  <img src="example.png" />
</p>
$N(x,y,\lambda) = N_0 + \sin x \sin y \sin A*\lambda$<br /><br />
$N$: Refractive index<br />
$N_0$: Original material refractive index<br />
$A$: Proportionality constant<br />
$\lambda$: Wavelength<br />
# The Physics
I simply used the Snell-Descartes law on refraction: $N_0 \sin \theta_0 = N_1 \sin \theta_1$ <br />
I then simply trace the light ray using Euler's Method.<br />
You can edit the code to create different light paths.<br />
# File format
The generated images are in the ppm file format:
# Final message
Enjoy C:<br />
