<?php
class private_party_group extends active_record {
  protected $_table = "private_party_groups";

  public $name;
  public $child_of;
  public $created_date;
  public $created_id;

  private $_parent;

  /**
   * @param $uid
   *
   * @return array of private_party_group_user
   */
  static public function get_by_user($uid) {
    return private_party_group_user::search()->where('uid', $uid)->exec();
  }

  /**
   * Does this object have a parent?
   *
   * @return bool
   */
  public function has_parent() {
    if ($this->child_of > 0) {
      if ($this->get_parent() instanceof private_party_group) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  /**
   * Get parent object, or false if none.
   *
   * @return bool|private_party_group
   */
  public function get_parent() {
    if (!$this->_parent) {
      $this->_parent = private_party_group::search()->where('gid', $this->child_of)->execOne();
    }

    return $this->_parent;
  }

  /**
   * Return a sanitised name
   *
   * @return string
   */
  public function get_name() {
    return ucwords(strtolower($this->name));
  }

  /**
   * Translate out a name from groups, such as:
   *   "Regions / North East & Yorkshire / West Yorkshire"
   * Rather than just:
   *   "West Yorkshire"
   *
   * @return string
   */
  public function __toString() {
    $name = $this->get_name();
    if ($this->has_parent()) {
      $name = $this->get_parent()->__toString() . " / " . $name;
    }

    return $name;
  }

  /**
   * Is the passed group in our group tree?
   *
   * @param private_party_group $group
   */
  public function in_tree(private_party_group $group) {
    if ($group->gid == $this->gid) {
      return true;
    }
    if ($this->has_parent()) {
      return $this->get_parent()->in_tree($group);
    }

    return false;
  }

  /**
   * Get the children of this item
   *
   * @return array of private_party_group
   */
  public function get_children() {
    return private_party_group::search()
      ->where('child_of', $this->gid)
      ->exec();
  }

  /**
   * Get the Users in this Group.
   *
   * @return array of user_active_record
   */
  public function get_users() {
    $users = array();
    foreach (private_party_group_user::search()->where('gid', $this->gid)->exec() as $group_user) {
      $users[$group_user->uid] = $group_user->get_user();
    }

    // Process child groups
    $child_groups = $this->get_children();
    if (count($child_groups) > 0) {
      foreach ($child_groups as $child_group) {
        foreach ($child_group->get_users() as $user) {
          if ($user instanceof user_active_record) {
            $users[$user->uid] = $user;
          }
        }
      }
    }

    return $users;
  }

  /**
   * Does this group, or its children contain a given user?
   *
   * @param user_active_record $user
   * @param boolean            $check_children
   *
   * @return boolean
   */
  public function contains_user(user_active_record $user, $check_children = true) {
    $match = false;

    // Loop over the users attached to this group and decide if they match
    $potential_users = $this->get_users();
    foreach ($potential_users as $potential_user) {
      if ($potential_user instanceof user_active_record) {
        if ($user->uid == $potential_user->uid) {
          $match = true;
        }
      }
    }

    // Check our children recursively
    if ($check_children) {
      foreach ($this->get_children() as $child_group) {
        if ($child_group->contains_user($user)) {
          $match = true;
        }
      }
    }

    return $match;
  }
}
