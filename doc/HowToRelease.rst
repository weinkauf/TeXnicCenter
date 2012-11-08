Releasing TeXnicCenter
======================

1. ``hg up``
2. Edit ``cmake\TeXnicCenter.cmake`` appropriately.
3. Configure and generate the solution using CMake.
4. Build, test.
5. Make sure, the rest of the setup script looks still good. Test building the
   setup.
6. Merge default into stable::

    hg up stable && hg merge default && hg ci --m "merge with default"

7. Create a tag for the release.

  Namescheme::

    'Release' MajorVersion (Alpha|Beta|.) MinorVersion

  without any spaces.

  Examples::

    Release2Alpha1
    Release2Beta2
    Release2.0
    Release2.01

8. ``hg push``
9. Rebuild All
10. Create setup. Can be found in the build directory with proper name.
11. Test setup on a fresh Windows (or in a virtual machine).
12. AntiVirus check of the freshly installed TXC.
13. Write Release Notes. Use this as a startinig point (where
    ``Release2Alpha3`` stands for the tag of the last release)::

    hg log -f --no-merges -r "tagged(Release2Alpha3)":: --template "{desc}\n\n" > TXCReleaseNotes_2Alpha4.txt

14. Upload the release notes and the setup file to sf.net. Just click on
    "Files". It requires Firefox or something similar. Create a subfolder for
    the release. Place the release notes as readme.txt in this folder (can be
    renamed on the server). It will always be displayed when people go into
    that folder (reST could be used, too). If the installer is going to be the
    new default, set it in its properties.
15. Change the download links at texniccenter.org.
16. Publish news at texniccenter.org and latex-community.org. texniccenter.org
    requires FireFox to edit an article properly.
17. Add a new artifact group at sf.net for the User Reports. To do so, (try
    to) add a new report, click on the admin link next to the group drop-down
    box.
