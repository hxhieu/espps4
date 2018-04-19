<template>
  <tr class="payload-info">
    <td>
      <a :href="`${$config.host()}entry/${payload.key}`" class="payload-name">{{ payload.name }}</a>
    </td>
    <td class="gauge-col">
      <gauge-bar
          v-if="totalKb"
          :max-value="totalKb"
          :current-value="payload.sizeKb"
          :value-formatter="valueFormatter"
          :border-thickness="0"
          :border-gap="0"
          :bar-height="40">
      </gauge-bar>
    </td>
    <td class="delete-col">
      <span class="btn-delete" @click="deleteMe">x</span>
    </td>
  </tr>
</template>

<script>
import GaugeBar from './GaugeBar';
export default {
  name: 'payload-info',
  components: {
    [GaugeBar.name]: GaugeBar,
  },
  props: {
    payload: Object,
    totalKb: Number,
    valueFormatter: Function,
    deleteHandler: Function,
  },
  methods: {
    deleteMe() {
      if (confirm(`Delete payload ${this.payload.name} \r\nAre you sure?`)) {
        this.deleteHandler(this.payload.key);
      }
    },
  },
};
</script>

<style lang="less">
@import '../variables';
@text-size: 2em;

.payload-info {
  cursor: default;
  transition: @hover-transition;

  &:hover {
    background: @primary-colour;
    color: #fff;
    td {
      .btn-delete,
      .payload-name {
        color: #fff;
      }
    }
  }

  td {
    padding: 5px;
    border-bottom: 1px solid;
    &.gauge-col {
      width: 15%;
    }

    &.delete-col {
      width: @text-size;
      text-align: center;
    }

    .payload-name {
      padding: 0 20px;
      font-size: @text-size;
      text-decoration: none;
    }

    .btn-delete {
      cursor: pointer;
      text-decoration: none;
      font-size: @text-size;
      line-height: 1em;
      color: @secondary-colour;
      display: block;
    }
  }
}
</style>


