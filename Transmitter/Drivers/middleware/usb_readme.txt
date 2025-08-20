
La librairie USB ne peut pas être compilée pour gérer le mode DEVICE et le mode HOST.
Il faut fait un choix à la compilation entre ces deux modes...

Mode Device : la carte est considérée comme un périphérique USB par l'ordinateur sur laquelle on l'a branchée.
	Par exemple, elle peut se faire reconnaitre comme une souris, un clavier, ou un périphérique de stockage de masse.
	
Mode Host : la carte acceuille des périphériques USB. 
	Par exemple une clé USB, une souris...
	
	
Pour changer de mode :

Il faut activer la macro USE_DEVICE_MODE ou USE_HOST_MODE dans lib/middleware/usb_conf.h

Il faut ensuite désactiver la compilation de l'un des deux dossiers lib/middleware/usb_host ou lib/middleware/usb_device :
   1 - bouton droit sur le dossier à désactiver
   2 - Ressource configuration
   3 - Exclude from Build
   4 - Cocher les modes de compilations pour lesquels on souhaite cocher cette exclusion (tous...)
L'opération inverse (décocher...) permet de réactiver la compilation du mode souhaité.

