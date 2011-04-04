
NOTICE
_______________________________________________________________________________

Ce dictionnaire, appel� R�forme 1990, respecte les rectifications de
l'orthographe propos�es en 1990 par le Conseil sup�rieur � la langue fran�aise
et par l'Acad�mie fran�aise et consid�re les anciennes graphies comme erron�es.
http://www.orthographe-recommandee.info/

Attention ! Ce dictionnaire est pr�vu pour fonctionner avec Hunspell, le 
correcteur orthographique actuel d'OpenOffice.org, int�gr� depuis la version
2.0.2. Tout utilisateur d'une version ant�rieure devrait utiliser un dictionnaire
pr�vu pour Myspell.
Note : Firefox 3 utilise le correcteur Hunspell, Firefox 2 Myspell.

Contact: dico.savant<at>free.fr
Dicollecte: http://dico.savant.free.fr/

Remerciements � :
- Jean-Claude Michel (alias Vazkor), pour l'examen minutieux du dictionnaire
  et les innombrables corrections apport�es;
- Jean Beney, pour la comparaison avec le dictionnaire de l'ABU qui a permis de
  d�busquer maintes erreurs;
- Romain Muller, pour la v�rification des graphies sp�cifiques � la r�forme de
  l'orthographe de 1990;
- et � tous ceux qui ont apport� leur pierre � l'�difice.

_______________________________________________________________________________

LICENCES
_______________________________________________________________________________

Avec l'accord de l'auteur initial, Christophe Pythoud, ces dictionnaires
sont d�sormais distribu�s sous triple licence : LGPL, GPL, MPL.

Licence GPL 2    ou sup�rieure   http://www.gnu.org/licenses/gpl-2.0.html
Licence LGPL 2.1 ou sup�rieure   http://www.gnu.org/licenses/lgpl-2.1.html
Licence MPL 1.1  ou sup�rieure   http://www.mozilla.org/MPL/MPL-1.1.html

La premi�re version du dictionnaire MySpell pour OpenOffice.org a �t� cr��e 
automatiquement � partir de la convertion du fichier affix et des listes 
de mots cr��s par Christophe Pythoud pour Ispell. 
Ces fichiers ont �t� publi�s dans la version 1.0.1 de Fran�ais-GUTenberg et
�taient soumis � la licence GPL version 2.

_______________________________________________________________________________

CHANGELOG
_______________________________________________________________________________


##### VERSION 2.3.1 - avril 2008 ##############################################
�tablie par O.R.

----- Modifications du dictionnaire -------------------------------------------

* D�pliage des lemmes pr�fix�s avec C D N P R O T.
  A permis d'�liminer bon nombre d'erreurs et de redondances.
* Mise � jour avec les propositions du site "Dicollecte".
  
----- Modifications du fichier des affixes ------------------------------------

* Suppression des drapeaux de pr�fixation : C D N P R O T


##### VERSION 2.2.0 - mars 2008 ###############################################
##### VERSION 2.1.0 - f�vrier 2008 ############################################
�tablies par O.R.

----- Modifications du dictionnaire -------------------------------------------

* Ajout de communes fran�aises (villes de plus de 20000 habitants)
* Mise � jour avec les entr�es propos�es sur le site "Dicollecte".
* Diverses corrections
* Quelques ajouts

----- Modifications du fichier des affixes ------------------------------------

* Modifications du m�canisme de suggestion :
  + REP ai �
  + REP � ai
  + REP ai �
  + REP � ai
  + REP ai �
  + REP � ai
  + REP ei �
  + REP � ei
  + REP ei �
  + REP � ei
  + REP ei �
  + REP � ei
  + REP ss c
  + REP c ss
  + REP k qu
  + REP qu k
  + REP x ct
  + REP ct x
  + REP ss �
  + REP � ss
  + REP o au
  + REP au o
  + REP o eau
  + REP eau o
  + REP disez dites
  + REP fesez faites
  + REP faisez faites
  + REP d�cr�dibilis discr�dit
  + REP antitartre d�tartrant
  Exemples:
    lumin�re   > luminaire
    p�gne      > peigne
    impreigner > impr�gner
* Modification du drapeau y
  > SFX y   er         �-je       er
  
  
##### VERSION 2.0.5 - d�cembre 2007 ###########################################
##### VERSION 2.0.4 - d�cembre 2007 ###########################################
##### VERSION 2.0.3 - d�cembre 2007 ###########################################
�tablies par O.R.

* Mise � jour avec les entr�es propos�es sur le site "Dicollecte".
* Diverses corrections

  
##### VERSION 2.0.2 - novembre 2007 ###########################################
�tablie par O.R.

----- Modifications du dictionnaire -------------------------------------------

* Suppression de milliers d'entr�es redondantes (notamment avec les d�clinaisons
  engendr�es par les drapeaux p, q).
* Corrections des mots avec ligatures ('oe', 'ae').
* Ordonnancement des drapeaux (et effacement des doublons).
* Des centaines de nouvelles entr�es.
* Des centaines de corrections.
* Mise � jour avec les entr�es propos�es sur le site "Dicollecte".
* Reprise partielle des corrections et ajouts du dictionnaire de Vazkor
  du 08 septembre 2007. http://perso.latribu.com/rocky2/mydico.html
  La plupart des ajouts et des modifications ont �t� repris (environ 3200 sur 4900).
  Seules les suppressions des entr�es erron�es ont �t� reprises (environ 800 sur 12300).
  Les termes rares, scientifiques, math�matiques, les mots compos�s, etc. n'ont
  pas �t� effac�s.
  Quelques erreurs ont �t� corrig�es.
* S�paration en deux lemmes d'entr�es qui sont � la fois verbe et nom commun.
  Exemple : espionne/LMjnmtlFu
          > espionne/LMF
          > espionne/jnmtlu
  Permet r�duire les redondances et d'�liminer des d�clinaisons erron�es.
  [seulement dans la version Hunspell]
* Comparaison du dictionnaire avec celui de l'ABU, ce qui a permis de corriger
  des centaines d'erreurs et de combler des oublis. (Jean Beney)
* V�rification des nouvelles orthographes recommand�es. (Romain Muller)
  http://www.orthographe-recommandee.info/
  
----- Modifications du fichier des affixes ------------------------------------

* ajout des lignes MAP, REP et WORDCHARS
  >> meilleures suggestions de correction
* modification du drapeau D
  > PFX D   0          d�         [^a��e���i�o�uh��]
  > PFX D   0          d�s        [a��e���i�o�uh��]
* modification du drapeau R
  > PFX R   0          re         [^a��e���i�o�uhs��]
  > PFX R   0          r�         [a��e���i�o�uh��]
* modification du drapeau O
  > PFX O   0          r�         [^a��e���i�o�uhs��]
  > PFX O   0          r          [a��e���i�o�uh��]
* modification du drapeau L
  > PFX L   0          l'         [a��e���i�o�uyh��A��E���I�O�UYH��]
* modification du drapeau D
  > PFX M   0          d'         [a��e���i�o�uyh��A��E���I�O�UYH��]
* modification du drapeau Q
  > PFX Q   0          qu'        [a��e���i�o�uyh��A��E���I�O�UYH��]
  > PFX Q   0          quoiqu'    [a��e���i�o�uyh��A��E���I�O�UYH��]
  > PFX Q   0          puisqu'    [a��e���i�o�uyh��A��E���I�O�UYH��]
  > PFX Q   0          lorsqu'    [a��e���i�o�uyh��A��E���I�O�UYH��]
* modification du drapeau U
  > PFX U   0          jusqu'     [a��e���i�o�uh��]
* modification du drapeau X
  > SFX X   0          x          [aeo�]u
* modification du drapeau j
  > PFX j   0          j'         [a��e���i�o�uyh��]
* modification du drapeau n
  > PFX n   0          n'         [a��e���i�o�uyh��]
  > PFX n   0          qu'        [a��e���i�o�uyh��]
* modification du drapeau m
  > PFX m   0          m'         [a��e���i�o�uyh��]
* modification du drapeau t
  > PFX t   0          t'         [a��e���i�o�uyh��]
* modification du drapeau l
  > PFX l   0          l'         [a��e���i�o�uyh��]
* modification du drapeau s
  > PFX s   0          s'         [a��e���i�o�uyh��]
* modification du drapeau F
  - SFX F   �e         �          �e
  - SFX F   �le        el         �le
  - SFX F   �le        els        �le
  > SFX F   ne         n          [aiou]ne
  > SFX F   e          s          [aiou]ne
  > SFX F   re         r          [aiu�y]re
  > SFX F   e          s          [aiu�y]re
  + SFX F   esse       e          esse
  + SFX F   he         h          [ut]he
  + SFX F   e          s          [ut]he
  + SFX F   ke         k          ke
  + SFX F   e          s          ke
* modification du drapeau x
  > le champ conditionnel 'ai' a �t� remplac� par 'rai'
* modification du drapeau p
  > SFX p   e          �rent      [^�][^�].e
  > SFX p   0          ra         e
  > SFX p   0          ront       e
  > SFX p   0          rait       e
  > SFX p   0          raient     e
  + SFX p   ie         y�rent     [aou]ie
  + SFX p   elle       el�rent    elle
  + SFX p   �re        �r�rent    �re
  + SFX p   �ge        �g�rent    �ge
  + SFX p   �ve        ev�rent    �ve
  + SFX p   �ne        �n�rent    [^mr]�ne
  + SFX p   �ne        en�rent    [mr]�ne
  + SFX p   �te        �t�rent    [^h]�te
  + SFX p   �te        et�rent    ch�te
  + SFX p   �le        el�rent    �le
  + SFX p   �che       �ch�rent   �che
  + SFX p   �que       �qu�rent   �que
  + SFX p   �tre       �tr�rent   �tre
  + SFX p   �gne       �gn�rent   �gne
  + SFX p   �gre       �gr�rent   �gre
  + SFX p   �vre       �vr�rent   �vre
  
  L�gende : 
  > lignes modifi�es
  + lignes ajout�es
  - lignes supprim�es


  
###############################################################################

Ce dictionnaire MySpell inclut les propositions de rectifications de l'orthographe, �mises en 1990 par le Conseil sup�rieur � la langue fran�aise et par l'Acad�mie fran�aise, et consid�re comme fautives les anciennes graphies. Il a �t� cr�� � partir du dictionnaire MySpell fr_FR 1.0.1 de Fran�ais-GUTenberg (voir ci-dessous).

Septembre 2006
~~~~~~~~~~~~~~
- int�gration au projet lingucomponent d'OpenOffice.org (lgodard@indesko.com)

Juin 2006
~~~~~~~~~
- Ajouts de nombreux noms propres et de n�ologismes
- Correction des pluriels de quelques mots en -al (naval, banal ...)
- Correction de la 2e personne du pluriel de l'indicatif des verbes d�riv�s de dire (pr�dire, d�dire, m�dire ...)
- Restructuration du fichier d'instructions *.aff, suppression des param�tres inutilis�s, suppression des param�tres utiles � un seul verbes
- Regroupement des verbes avec leurs d�riv�s � pr�fix commen�ant par une consonne (contre-, d�-, r�-, pr�-, sur-)


Octobre 2005
~~~~~~~~~~~~
Reprise de la liste des mots du dictionnaire MySpell fr_FR 1.0.1 de Fran�ais-GUTenberg et adaptation de celle-ci (ajouts et/ou suppresions de mots) afin de la faire correspondre aux graphies renouvel�es propos�es en 1990 par deux organes officiels de l'�tat fran�ais (pour plus d'info, voir : http://www.orthographe-recommandee.info/)
Pour toutes propositions, remarques, critiques, ... : mat.schopfer@bluewin.ch


Historique du dictionnaire MySpell fr_FR de Fran�ais-GUTenberg :
----------------------------------------------------------------

version 1.0.1
Version corrig�e de la liste de mots avec regroupement des mots �quivalents afin 
de supprimer des fautes lors de la correction (les mots sont consid�r�s comme faux
si ils existent � double dans la liste de mots)

version 1.0.0
Version du dictionnaire MySpell fr_FR g�n�r�e automatiquement depuis � partir de la
convertion du fichier affix et des listes de mots cr��s par Christophe Pythoud pour
Ispell
