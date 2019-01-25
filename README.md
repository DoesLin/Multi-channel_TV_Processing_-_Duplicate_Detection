# Real_Time_Multi_channel_TV_Processing_with_Embedded_TNT_TV_Tuner_and_FFmpeg_Library

Les canaux / chaines TV sont diffusés au travers des réseaux internet (box / décodeur) mais majoritairement via la TNT (Télévision Numérique Terrestre) [1]. En effet, la plupart des opérateurs internet rediffusent les chaînes depuis le réseaux TNT [2]. Les équipements / installations des particuliers disposent de décodeurs / récepteurs pour traiter les flux TNT pour les postes TV, mais ceci peut également être récupérés depuis des machines (fixe, portable, smartphone / tablette, console, etc.). La TNT est diffusée sous norme de compression MPEG-4 AVC (H.264) [1] et il existe de multiple carte d'acquisition (ou tuner embarqué, carte tv) permettant la capture des flux TV TNT sur machine [3, 4, 5].

L'objectif de ce sujet est double. Dans un premier temps il sera demandé de faire le point sur les solutions de capture des flux TNT sur machine fixe, en particulier pour la lecture de canaux multiple (plusieurs chaines en simultanée depuis une même machine). Dans un deuxième temps, à partir d'une solution clé en main (e.g. lecteur TNT sous clé USB) il sera demandé de mettre en œuvre la librairie standard ffmpeg [6] pour la lecture et le posttraitement (enregistrement, mise à l'échelle, conversion, etc.) d'un flux TV TNT en temps réel.

[1]https://fr.wikipedia.org/wiki/T%C3%A9l%C3%A9vision_num%C3%A9rique_terrestre_en_France

[2]	http://www.tomshardware.fr/articles/media-center-mini-pc-windows,2-2459-7.html		

[3]	https://doc.ubuntu-fr.org/liste_cartes_tv		

[4]	https://support.xbox.com/fr-FR/xbox-one/accessories/xbox-one-digital-tv-tuner		

[5]	http://chezcyril.over-blog.com/2017/04/un-recepteur-tnt-hd-pour-votre-smartphone-ou-tablette-android-a-moins-de-20.html

[6]	https://www.ffmpeg.org/
