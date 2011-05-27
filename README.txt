 drush (git) pull.
 
  One of the problems working combine features module w/ Git is, 
  if the pulling from git occur before updating all features changes to code.
  
  Well, let say we got some feature changes comming form the git pull, 
  and and we got a feature in override state (local changes which not exported to code yet).
  There two options heading us now:
   - Revert the feature and ignore all of the settings changes in the DB
   - Recreate the feature (export the local changes to code) and override some else's job.
  
  This drush command is trying to help enforce the very basic procedures: 
  - "Always export your feature to code BEFORE you pulling from git".
  - "Always revert your feature from code AFTER pulling (drush fr)".
  
  It's doing it by checking the features state before pulling and stop you from continue,
  in case one or more features is in override state.
  
  (although if you still want to pull knowing not all features are in default state, 
  you can use the drush pull force command.)