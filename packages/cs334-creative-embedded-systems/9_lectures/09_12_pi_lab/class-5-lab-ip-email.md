Lab: Pair-programming (with one group of three, or *one* very lonely number…)

Primary Objejective: configure Pi to auto-connect to the wifi and send an email with the current IP address. After enabling SSH this will allow you to access the pi quickly and easily from your laptop terminal.

Please keep track of the tasks you have accomplished (with a check mark) and turn these papers in at the end of the lab. We will use them to determine whether future labs require more or less activities/configuration. This file will be uploaded as an electronic resource to Canvas for you to continue to reference.


1) Register eth and wlan MAC addresses with Yale:
    - $ ifconfig
    - https://regvm3.its.yale.edu/register
    - paste MAC with description of your Pi

2) Review best practices for development
    - dev on local machine
    - deploy to prod (Pi)
    - git
    - scp
    - ?

3) How-to install software
    - $ sudo apt-get install SOFTWARE

4) Change user/pw on pi (this is best practice)
    - terminal: $passwd

3) Enable SSH for remote access
    - from local card reader:
        - [from /boot] $ touch ssh
        - use '$ man touch' to see what this does
    - From Pi GUI
        - Launch Raspberry Pi Configuration from the Preferences menu.
        - Navigate to the Interfaces tab.
        - Select Enabled next to SSH.
        - Click OK.

4) Write a script to send an email with the Rpi's IP every time it boots
depending on how you do this, you will need *some or none* of the following:
    - ssmtp
    - mail/mailx (mailutils)
    - python
    - You *will* need to use a gmail account
    - best practice is to not use your account passwd but to generate an application password from here: https://support.google.com/accounts/answer/185833?hl=en

5) Use SSH to get remote access to the Pi, then:
    - download the script (from or to)
    - configure rc.local to run the script on boot
    - test and confirm
