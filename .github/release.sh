#!/bin/bash


function c4_release_create()
{
    ver=$(_c4_validate_ver $1)
    branch=$(_c4_validate_branch $2)
    ( set -x ; set -e ; c4_release_bump $ver ; c4_release_commit $ver $branch )
}

function c4_release_redo()
{
    ver=$(_c4_validate_ver $1)
    branch=$(_c4_validate_branch $2)
    ( set -x ; set -e ; c4_release_delete $ver ; c4_release_bump $ver ; c4_release_amend $ver $branch )
}

function c4_release_bump()
{
    ver=$(_c4_validate_ver $1)
    ( set -x ; set -e ; tbump --non-interactive --only-patch $ver ; )
}

function c4_release_commit()
{
    ver=$(_c4_validate_ver $1)
    branch=$(_c4_validate_branch $2)
    ( \
        set -x ; \
        set -e ; \
        git add -u ; \
        git commit -m "v$ver" ; \
        git tag --annotate --message "v$ver" "v$ver" ; \
        git push origin $branch ; \
        git push --tags origin $branch \
    )
}

function c4_release_amend()
{
    ver=$(_c4_validate_ver $1)
    branch=$(_c4_validate_branch $2)
    ( \
        set -x ; \
        set -e ; \
        git add -u ; \
        git commit --amend -m "v$ver" ; \
        git tag --annotate --message "v$ver" "v$ver" ; \
        git push -f origin $branch ; \
        git push -f --tags origin $branch \
    )
}

function c4_release_delete()
{
    ver=$(_c4_validate_ver $1)
    ( \
        set -x ; \
        set -e ; \
        git tag -d v$ver ; \
        git push origin :v$ver ; \
    )
}

function _c4_validate_ver()
{
    ver=$1
    if [ -z "$ver" ] ; then exit 1 ; fi
    echo $(echo $ver | sed "s:v\(.*\):\1:")
}

function _c4_validate_branch()
{
    branch=$1
    if [ -z "$branch" ] ; then exit 1 ; fi
    echo $branch
}
