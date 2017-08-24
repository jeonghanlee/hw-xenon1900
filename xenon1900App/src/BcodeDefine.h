#ifndef INC_BCODEDEFINE_H
#define INC_BCODEDEFINE_H

/* Predefined PREFIX for Barcode */
static const char * const ff="FF"; /* Formfactor */
static const char * const vd="VD"; /* Vendor     */
static const char * const lo="LO"; /* Location   */
static const char * const st="ST"; /* Status     */
static const char * const mo="MO"; /* Model      */
static const char * const oo="00"; /* Place holder */

static const char * const hs="HS"; /* Hash number per each SN                       */
static const char * const cl="CL"; /* Clear all scanned PVs                         */

static const char * const le="LE"; /* Enable  Label Printing after JIRA action (JC) */
static const char * const ld="LD"; /* Disable Label Printing after JIRA action (JC) */
static const char * const lp="LP"; /* Print the existent label */
static const char * const jc="JC"; /* Create an JIRA issue                          */
static const char * const ju="JU"; /* Update an JIRA issue  */
static const char * const jd="JD"; /* Delete an JIRA issue  */


#endif
