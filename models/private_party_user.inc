<?php
class private_party_user extends user_active_record{

  /**
   * Does this user have a specific group?
   * @param $group_name
   */
  public function has_group($group_name){
    // Administrator has all the things.
    if($this->uid == 1){
      return TRUE;
    }

    foreach($this->get_groups() as $group){
      /* @var $group_user private_party_group */
      if($group->name == $group_name){
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Get the groups attached to this private_party_users
   * @return array of groups
   */
  public function get_groups(){
    $group_users = private_party_group_user::search()->where('uid', $this->uid)->exec();
    $merged_groups = array();
    foreach($group_users as $group_user){
      /* @var $group_user private_party_group_user */
      $groups = $group_user->get_groups();
      $merged_groups = array_merge($merged_groups, $groups);
    }
    return $merged_groups;
  }

  /**
   * Get the currently logged in user.
   * Will return anonymous account if fail.
   *
   * @return private_party_user
   */
  static public function current(){
    return parent::current();
  }
}